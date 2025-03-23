/**
 * @file dir.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 进程打开的目录操作的源文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#include "dir.h"

#include "defs.h"
#include "inode.h"

#include <linux/kernel.h>
#include <linux/buffer_head.h>


/**
 * @brief 由进程打开的目录（用 file 结构描述）的操作接口。
 */
struct file_operations nvmixDirFileOps = {
    .owner = THIS_MODULE,
    // read 是 2.6 版本以前的旧接口，iterate 和 iterate_shared 是 2.6 以后的接口，都可用来遍历目录。
    // read 不支持并发访问。
    // iterate 是独占式遍历，持有目录的 inode 互斥锁，支持并发访问，确保遍历期间目录结构不会被修改。
    // iterate_shared 是共享式遍历，仅持有目录的 inode 共享锁，允许其他进程并发遍历同一目录。
    // 优先使用 iterate_shared，未实现则退回 iterate。
    .read = generic_read_dir,
    // TODO 后续考虑共享式遍历的 iterate_shared 的修改实现。
    .iterate = nvmixReaddir,
};


int nvmixReaddir(struct file *pDirFile, struct dir_context *pCtx)
{
    struct inode *pParentDirInode = NULL;
    struct super_block *pSb = NULL;
    struct NvmixInodeHelper *pNih = NULL;
    struct buffer_head *pBh = NULL;
    struct NvmixDentry *pNde = NULL;
    int res = 0;
    int isOver = 0;


    pParentDirInode = pDirFile->f_inode;
    pNih = NVMIX_I(pParentDirInode);

    pSb = pParentDirInode->i_sb;
    // sb_bread() 用于从磁盘读取指定块的数据到内存缓冲区，第一个参数是超级块指针，第二个参数是要读取的逻辑块号。返回缓冲区的头指针。
    // 目录的数据块存储的设计见 defs.h，这样能更方便理解下面的逻辑。
    pBh = sb_bread(pSb, pNih->m_dataBlockIndex);
    if (!pBh)
    {
        pr_err("nvmixfs: could not read data block.\n");

        res = -ENOMEM;
        goto ERR;
    }

    // dir_context 是内核用于目录遍历操作的关键数据结构。它封装了遍历目录时的上下文信息。主要作用是在多次调用目录遍历函数（如 .iterate 或 .iterate_shared）时，保存遍历的进度和状态，确保每次调用能正确继续上一次的位置。
    for (; pCtx->pos <= NVMIX_MAX_ENTRY_NUM; ++pCtx->pos)
    {
        // 当前处理的对象是目录（特殊文件），包括普通目录，. 和 .. 等。目录不存在数据信息，但与文件一样有 inode 以及 inode 的相关元数据。在磁盘块中目录的数据应额外存储目录下文件的一些信息，至少应关联文件名和 inode 号，方便接口例如 readdir()、lookup() 等使用。这也是内存中的 vfs dentry 做的事情。
        // 由此，目录的磁盘块存储的是一个 NvmixDentry 数组，记录的信息前面提到了。通过 pBh->b_data 获得 NvmixDentry 数组的头指针，然后加上偏移量即可得到每条目录项的信息。
        // [Entry 0] -> ino = 5, name = "file1"
        // [Entry 1] -> ino = 0, name = ""（无效条目，0 == m_ino 时跳过）
        // [Entry 2] -> ino = 7, name = "dir2"
        pNde = (struct NvmixDentry *)(pBh->b_data) + pCtx->pos;

        // inode 为 0 无效条目，需跳过。
        if (0 == pNde->m_ino) continue;

        // dir_emit() 用于在实现文件系统的 readdir 操作时，将有效的目录项信息填充到用户空间的缓冲区。返回非 0 值标识用户空间缓冲区已满或发生错误，此时需记录当前位置并跳出循环，以方便下次读取剩余条目。
        isOver = dir_emit(pCtx, pNde->m_name, NVMIX_MAX_NAME_LENGTH, pNde->m_ino, DT_UNKNOWN);
        if (isOver)
        {
            pr_info("nvmixfs: read %s from directory %s, ctx->pos: %lld\n", pNde->m_name, pDirFile->f_path.dentry->d_name.name, pCtx->pos);

            ++pCtx->pos; // 当前 pos 位置读取完毕，跳到下个位置并跳出循环。

            break;
        }
    }


ERR:
    // 释放缓冲区。
    brelse(pBh);
    pBh = NULL;


    return res;
}
