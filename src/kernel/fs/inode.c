/**
 * @file inode.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 文件和目录的 inode 操作源文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#include "inode.h"

#include "defs.h"
#include "fs.h"

#include <linux/cred.h>
#include <linux/buffer_head.h>


/**
 * @brief 文件 inode 操作的注册接口。
 */
struct inode_operations nvmixFileInodeOps = {
    .getattr = simple_getattr,
};

/**
 * @brief 目录 inode 操作的注册接口。
 */
struct inode_operations nvmixDirInodeOps = {
    .lookup = nvmixLookup,
    .create = nvmixCreate,
    .unlink = nvmixUnlink,
    .mkdir = nvmixMkdir,
    .rmdir = nvmixRmdir,
};

extern struct file_operations nvmixFileFileOps;

extern struct file_operations nvmixDirFileOps;

extern struct address_space_operations nvmixAops;


/**
 * @brief 在父目录下创建新 inode。
 * @param pParentDirInode 父目录的 inode 指针。
 * @return 新 inode 的指针。
 * @details 此函数的语义是在父目录下创建新的通用的 inode，并做基本通用项的初始化（具体见下面）。参数 struct inode* pParentDirInode 代表父目录。在 create() 语义下返回的 inode 是一个文件。创建目录有函数 mkdir()，里面也会调用此函数，在那的语义返回的 inode 就是一个目录。
 * @details static 静态函数只能被当前源文件使用，类似于 C++ 类中的私有成员函数。不建议声明在头文件中，因为头文件可能被其他文件包含，可能会出现未知问题，同时可能也会降低链接速度。
 * @details inline 即内联，建议放在很少行数（最好是一行）的函数使用，在调用函数的时候会类似宏一样直接展开，不用走函数调用栈，提高效率。但如果函数过长，不建议加上 inline，这样反而会导致代码冗余。
 */
static struct inode *nvmixNewInode(struct inode *pParentDirInode);

/**
 * @brief 在新创建 inode 的时候，维护父目录的目录项数组的信息。
 * @param pDentry 新创建目录项的 dentry 指针。
 * @param pInode 新创建目录项的 inode 的指针。
 * @return 成功返回 0，失败返回非 0。
 * @details 参数用新创建目录项的 dentry 而不是直接传入父目录，是因为需要目录项名称 pDentry->d_name.name。
 */
static int nvmixUpdateParentDirDentry(struct dentry *pDentry, struct inode *pInode);

/**
 * @brief 给定目标 dentry，找到磁盘对应的 NvmixDentry 结构。
 * @param pDentry 目标 dentry 的指针。
 * @param ppBh 磁盘缓冲区头的二级指针。
 * @return 成功返回目标指针，失败返回 NULL。
 * @details 为什么会有第二个参数，且为什么是二级指针，请参考函数定义的注释。
 */
static struct NvmixDentry *nvmixFindDentry(struct dentry *pDentry, struct buffer_head **ppBh);

/**
 * @brief 在父目录中创建新文件或目录的节点。
 * @param pParentDirInode 父目录的 inode 指针。
 * @param pDentry 新创建的节点的 dentry 指针。
 * @param mode 创建模式参数。
 * @param excl 独占创建标志，若设置为 true 要求目标必须不存在。暂未用到。
 * @return 成功返回 0，失败返回非 0。
 * @details 接口的参数逆天。pParentDirInode 是父目录的 inode 节点，在函数里我需要手动创建新的 vfs inode。而 pDentry 却是新 inode 节点对应的 dentry 对象，内核帮我创建好了。很容易误解为父目录的 dentry，我们需要自己手动创建 dentry，但是内核似乎并没有这种函数。
 */
static int nvmixMknod(struct inode *pParentDirInode, struct dentry *pDentry, umode_t mode, bool excl);


struct dentry *nvmixLookup(struct inode *pParentDirInode, struct dentry *pDentry, unsigned int flags)
{
    struct super_block *pSb = NULL;
    struct NvmixDentry *pNd = NULL;
    struct inode *pInode = NULL;
    struct buffer_head *pBh = NULL;


    pSb = pParentDirInode->i_sb;
    // 继承根目录的 dentry_operations 操作。我很疑惑既然内核都帮我创建出 pDentry 了，为什么不帮我们处理好这部分逻辑。
    pDentry->d_op = pSb->s_root->d_op;

    pr_info("nvmixfs: start looking up dentry %s\n", pDentry->d_name.name);

    // 通过 vfs dentry 找到磁盘块上该目录的 NvmixDentry 信息。
    pNd = nvmixFindDentry(pDentry, &pBh);
    // 注意未找到并不代表失败需要报错，只是代表 dentry 并无对应 inode，将其置为负状态即可（下面的 d_add()）。
    if (!pNd)
    {
        pr_info("nvmixfs: could not find target dentry in directory.\n");
    }
    else
    {
        pr_info("nvmixfs: found entry successfully: name: %s, ino: %ld\n", pNd->m_name, pNd->m_ino);

        // 通过 super_block 和全局唯一 inode 号找到对应 inode 结构。
        pInode = nvmixIget(pSb, pNd->m_ino);

        // ERR_CAST() 将错误指针转化为 void * 类型。
        if (IS_ERR(pInode)) return ERR_CAST(pInode);
    }

    // d_add() 函数用于将 dentry 绑定到关联的 inode，并将该 dentry 添加到哈希队列中，以便后续快速查找。
    // 如果 pInode 为空，即走上面 pNd 为空代表找不到匹配的 dentry 和 inode 的分支，此时的 pDentry 为负状态。即当文件不存在时，负状态的 dentry 会被缓存，避免重复触发实际文件系统的查找操作。多次访问一个不存在的文件，负状态的 dentry 会直接返回 ENOENT。因此上面的两个分支都会走该函数。
    d_add(pDentry, pInode);

    brelse(pBh);
    pBh = NULL;


    // 大多数情况返回 NULL 表示成功。返回非空的 struct dentry * 代表是可能一些特殊情况，这里暂未遇到。
    return NULL;
}

int nvmixCreate(struct inode *pParentDirInode, struct dentry *pDentry, umode_t mode, bool excl)
{
    int res = 0;


    res = nvmixMknod(pParentDirInode, pDentry, mode | S_IFREG, excl);
    if (0 != res)
    {
        pr_info("nvmixfs: failed to create new file.\n");

        goto ERR;
    }

    pr_info("nvmixfs: created new file successfully.\n");


ERR:
    return res;
}

int nvmixUnlink(struct inode *pParentDirInode, struct dentry *pDentry)
{
    struct inode *pInode = NULL;
    struct NvmixInodeHelper *pNih = NULL;
    struct buffer_head *pBh = NULL;
    struct super_block *pSb = NULL;
    struct NvmixDentry *pNd = NULL;
    struct NvmixSuperBlockHelper *pNsbh = NULL;
    struct NvmixSuperBlock *pNsb = NULL;
    int i = 0;
    int res = 0;


    // vfs 部分的代码参考 simple_unlink() 的实现。
    pInode = pDentry->d_inode;
    // 更新时间戳。
    pInode->i_ctime = current_time(pInode);
    pParentDirInode->i_ctime = current_time(pInode);
    pParentDirInode->i_mtime = current_time(pInode);

    // 将磁盘上父目录的目录项数组读到缓存中。
    pNih = NVMIX_I(pParentDirInode);
    pSb = pParentDirInode->i_sb;

    pBh = sb_bread(pSb, pNih->m_dataBlockIndex);
    if (!pBh)
    {
        pr_err("nvmixfs: could not read data block.\n");

        res = -EIO;
        goto ERR;
    }

    // 找到对应目录项并清除。
    for (i = 0; i < NVMIX_MAX_ENTRY_NUM; ++i)
    {
        pNd = (struct NvmixDentry *)(pBh->b_data) + i;

        // 留意 nvmixFindDentry() 中类似的部分，那里不能判断 pNd->m_ino == pInode->i_ino。这里可以，因为这里的 pDentry 与 pInode 已绑定好，是完整的。
        if ((0 != pNd->m_ino) && (pNd->m_ino == pInode->i_ino) && (0 == strcmp(pNd->m_name, pDentry->d_name.name)))
        {
            memset(pNd->m_name, 0, NVMIX_MAX_NAME_LENGTH);
            pNd->m_ino = 0;

            break;
        }
    }

    mark_buffer_dirty(pBh);


    // 既然删除了 inode，超级块的 m_imap 位图信息需要更新，否则空占 inode。inode 区的内容倒不需要清空，新创建的时候覆盖即可。
    // 磁盘上超级块区的缓冲区指针一直存在于内存中，被 NvmixSuperBlockHelper 维护，不需要手动创建和释放。
    pNsbh = (struct NvmixSuperBlockHelper *)(pSb->s_fs_info);
    pNsb = (struct NvmixSuperBlock *)(pNsbh->m_pBh->b_data);
    // pNsb = (struct NvmixSuperBlock *)(pNsbh->m_superBlockVirtAddr);

    test_and_clear_bit(pInode->i_ino, &pNsb->m_imap);

    pr_info("nvmixfs: m_imap in nvmixUnlink(): %ld\n", pNsb->m_imap);

    mark_buffer_dirty(pNsbh->m_pBh);

    pr_info("nvmixfs: unlinked file successfully.\n");


    // 减少文件的硬链接数。
    drop_nlink(pInode);
    // 释放目录项的引用。
    dput(pDentry);


ERR:
    brelse(pBh);
    pBh = NULL;


    return res;
}

int nvmixMkdir(struct inode *pParentDirInode, struct dentry *pDentry, umode_t mode)
{
    int res = 0;


    res = nvmixMknod(pParentDirInode, pDentry, mode | S_IFDIR, 0);
    if (0 != res)
    {
        pr_info("nvmixfs: failed to create new directory.\n");

        goto ERR;
    }

    pr_info("nvmixfs: created new directory successfully.\n");


ERR:
    return res;
}

int nvmixRmdir(struct inode *pParentDirInode, struct dentry *pDentry)
{
    int res = 0;


    // 首先检查目录是否为空。
    // simple_empty() 函数返回非 0 表示 true，即空，返回 0 表示 false，即非空。
    if (0 == simple_empty(pDentry))
    {
        res = -ENOTEMPTY;

        pr_info("nvmixfs: error when removing a directory cause not empty.\n");

        goto ERR;
    }

    // 以下步骤按照 simple_rmdir() 来的。
    drop_nlink(d_inode(pDentry));
    nvmixUnlink(pParentDirInode, pDentry);
    drop_nlink(pParentDirInode);

    pr_info("nvmixfs: removed directory successfully.\n");


ERR:
    return res;
}


struct inode *nvmixNewInode(struct inode *pParentDirInode)
{
    struct super_block *pSb = NULL;
    struct NvmixSuperBlockHelper *pNsbh = NULL;
    struct NvmixSuperBlock *pNsb = NULL;
    struct inode *pInode = NULL;
    unsigned long index = 0;


    pSb = pParentDirInode->i_sb;
    pNsbh = (struct NvmixSuperBlockHelper *)(pSb->s_fs_info); // pSb->s_fs_info 含义解释见 fs.c。
    pNsb = (struct NvmixSuperBlock *)(pNsbh->m_pBh->b_data);
    // pNsb = (struct NvmixSuperBlock *)(pNsbh->m_superBlockVirtAddr);

    // find_first_zero_bit() 是内核提供的一个位图操作函数，其作用是从给定的位图中查找第一个值为 0 的位，即未使用的空闲资源。m_imap 是我们自己定义的管理 inode 分配状态的位图信息，类型是 unsigned long，8 个字节，64 位。对本文件系统而言，使用低 32 位，每一位用于标识分配状态。
    index = find_first_zero_bit(&pNsb->m_imap, NVMIX_MAX_INODE_NUM);
    if (NVMIX_MAX_INODE_NUM == index)
    {
        pr_err("nvmixfs: no space left in imap.\n");

        goto ERR;
    }

    // 原子性地测试并设置位图中的目标位，维护 m_imap 状态。
    test_and_set_bit(index, &pNsb->m_imap);
    // 标记缓冲区为脏，表示内容已被修改，需要写回磁盘。
    mark_buffer_dirty(pNsbh->m_pBh);

    pr_info("nvmixfs: m_imap in nvmixNewInode(): %ld\n", pNsb->m_imap);


    pInode = new_inode(pSb); // 此函数创建新的 inode 结构，并关联到文件系统的超级块。

    // 初始化通用 vfs inode 的一些信息。
    // 注意，本函数返回的可能是文件或目录的 inode，这二者的 inode 中需要填充不同项的内容。但 nvmixNewInode() 的语义是创建一个通用 inode，故只处理通用的部分，这些赋值只能放在这里而不能放在外层的 nvmixCreate() 或 nvmixMkdir() 中。
    pInode->i_uid = current_fsuid();        // 用户所有者 ID。
    pInode->i_gid = current_fsgid();        // 组所有者 ID。
    pInode->i_ino = index;                  // inode 编号。
    pInode->i_mtime = current_time(pInode); // 修改时间 Modification Time，作用对象是文件内容。
    pInode->i_atime = current_time(pInode); // 访问时间 Access Time，作用对象是文件内容。
    pInode->i_ctime = current_time(pInode); // 变更时间 Change Time，作用对象是 inode 元数据。
    pInode->i_blocks = 0;                   // 初始化 inode 的数据块占用数。

    // 将新创建的 inode 插入到内核维护的全局 inode 哈希表。
    insert_inode_hash(pInode);


ERR:
    return pInode;
}

int nvmixUpdateParentDirDentry(struct dentry *pDentry, struct inode *pInode)
{
    struct inode *pParentDirInode = NULL;
    struct NvmixInodeHelper *pNih = NULL;
    struct super_block *pSb = NULL;
    struct buffer_head *pBh = NULL;
    struct NvmixDentry *pNd = NULL;
    int i = 0;
    int res = 0;


    pParentDirInode = pDentry->d_parent->d_inode;
    pNih = NVMIX_I(pParentDirInode);
    pSb = pParentDirInode->i_sb;

    pBh = sb_bread(pSb, pNih->m_dataBlockIndex);
    if (!pBh)
    {
        pr_err("nvmixfs: could not read data block.\n");

        res = -EIO;
        goto ERR;
    }

    // 找到第一个空闲的 NvmixDentry，即 inode 号为 0。
    for (i = 0; i < NVMIX_MAX_ENTRY_NUM; ++i)
    {
        pNd = (struct NvmixDentry *)(pBh->b_data) + i;

        if (0 == pNd->m_ino) break;
    }

    // 判断父目录是否已经满了。
    if (NVMIX_MAX_ENTRY_NUM == i)
    {
        pr_err("nvmixfs: failed to add link, because parent directory is full.\n");

        res = -ENOSPC;
        goto ERR;
    }

    // 填充磁盘上新目录项的元数据。
    pNd->m_ino = pInode->i_ino;
    strncpy(pNd->m_name, pDentry->d_name.name, NVMIX_MAX_NAME_LENGTH);

    // 修改父目录的 Modified Time 和 Changed Time，维护 vfs 的数据结构。
    pParentDirInode->i_mtime = current_time(pInode);
    pParentDirInode->i_ctime = current_time(pInode);

    mark_buffer_dirty(pBh);


ERR:
    brelse(pBh);
    pBh = NULL;


    return res;
}

struct NvmixDentry *nvmixFindDentry(struct dentry *pDentry, struct buffer_head **ppBh)
{
    struct inode *pParentDirInode = NULL;
    struct buffer_head *pBh = NULL;
    struct super_block *pSb = NULL;
    struct NvmixInodeHelper *pNih = NULL;
    struct NvmixDentry *pNd = NULL;
    struct NvmixDentry *pRes = NULL;
    int i = 0;


    // 读取父目录的目录项数组信息，并进行查找。
    pParentDirInode = pDentry->d_parent->d_inode;
    pSb = pParentDirInode->i_sb;
    pNih = NVMIX_I(pParentDirInode);

    pBh = sb_bread(pSb, pNih->m_dataBlockIndex);
    if (!pBh)
    {
        pr_err("nvmixfs: could not read data block.\n");

        goto ERR;
    }

    *ppBh = pBh;

    for (i = 0; i < NVMIX_MAX_ENTRY_NUM; ++i)
    {
        pNd = (struct NvmixDentry *)(pBh->b_data) + i;

        // 注意这个地方 pDentry 的语义。nvmixFindDentry() 在 nvmixLookup() 中使用，目的是给定目标 pDentry 找到磁盘的 NvmixDentry 结构。读 nvmixLookup() 的代码可知，此时 pDentry 尚未与 pInode 绑定，只有 pDentry->d_name 有值，pDentry->d_inode 是空指针 NULL，因此不能判断 pNd->m_ino == pDentry->d_inode->i_ino，否则会指针内存泄漏。切记！切记！
        if ((0 != pNd->m_ino) && (0 == strcmp(pNd->m_name, pDentry->d_name.name)))
        {
            pr_info("nvmixfs: found entry %s on position: %d\n", pNd->m_name, i);

            // 需要使用 pRes 存储返回值。因为 pNd 是遍历指针，如果找到条目了返回当然没问题，如果没找到就指向末元素，这样返回值就会出问题了。使用 pRes 则不会出现这样的问题，找到了赋值，找不到仍然是 NULL。
            pRes = pNd;

            break;
        }
    }


// 注意，这里一定不能释放 pBh，因为返回的 struct NvmixDentry * 依赖于 pBh 缓冲区的存在。如果释放了，传递出去以后就会内存泄露。nvmixFindDentry() 作为唯一被 nvmixLookup() 调用的工具函数，nvmixLookup() 需要用到 struct NvmixDentry * 指针，因此需要需要想办法将 pBh 传递出去。一种合适的方法就是使用 pBh 的指针，即二级指针 struct buffer_head **。至于 pBh 的释放留到 nvmixLookup() 中。
ERR:
    return pRes;
}

int nvmixMknod(struct inode *pParentDirInode, struct dentry *pDentry, umode_t mode, bool excl)
{
    int res = 0;
    struct inode *pInode = NULL;
    struct NvmixInodeHelper *pNih = NULL;


    pInode = nvmixNewInode(pParentDirInode);
    if (!pInode)
    {
        pr_err("nvmixfs: error when allocating a new inode.\n");

        res = -ENOMEM;
        goto ERR;
    }

    pInode->i_mode = mode;

    // 参考 ext4_create()，对以下操作做了注册。注意对应文件和目录分别处理。
    pInode->i_mapping->a_ops = &nvmixAops;

    if (S_ISREG(pInode->i_mode))
    {
        pInode->i_fop = &nvmixFileFileOps;
        pInode->i_op = &nvmixFileInodeOps;
    }
    else if (S_ISDIR(pInode->i_mode))
    {
        pInode->i_fop = &nvmixDirFileOps;
        pInode->i_op = &nvmixDirInodeOps;

        // 见 fs.c 的 nvmixIget() 函数注释。
        inc_nlink(pInode);
    }
    else
    {
        // 非普通文件或目录，暂不考虑。
    }


    pNih = NVMIX_I(pInode);
    pNih->m_dataBlockIndex = NVMIX_FIRST_DATA_BLOCK_INDEX + pInode->i_ino;

    // 将新 inode 关联到父目录的目录项 dentry 中，会维护并修改父目录项的一些信息。与下面的 d_instantiate() 作用不同，注意区分。
    // 注意此 pDentry 是 pInode 对应的 pDentry，而非父目录的 dentry，前面提到过。
    res = nvmixUpdateParentDirDentry(pDentry, pInode);
    if (0 != res)
    {
        // 减少 inode 的硬链接计数。
        inode_dec_link_count(pInode);
        // 释放 inode 的引用计数。
        iput(pInode);

        goto ERR;
    }

    // dentry 作用是关联 inode 和文件名。d_instantiate() 将 dentry 与 inode 绑定，使文件名正确指向文件。
    d_instantiate(pDentry, pInode);
    // 标记 inode 为脏，表示其元数据（如权限、大小）或数据已修改，需后续同步到磁盘。
    mark_inode_dirty(pInode);

    pr_info("nvmixfs: created new inode successfully, ino = %lu\n", pInode->i_ino);


ERR:
    return res;
}
