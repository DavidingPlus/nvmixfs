/**
 * @file inode.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 文件和目录的 inode 操作头文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_INODE_H_
#define _NVMIX_INODE_H_

#include <linux/fs.h>


/**
 * @struct NvmixInodeHelper
 * @brief 将内存中的 vfs inode 和 NVM 空间的元数据 NvmixInode 结构关联起来，媒介是逻辑块号 m_dataBlockIndex。
 * @details 此结构不是 NVM 上的元数据，NvmixInode 才是。NvmixInode 不能存储内存中 vfs 的数据结构，但需要一个东西将 vfs 和本文件系统自己的 inode 元数据 NvmixInode 联系起来。这就是本结构体的作用。
 */
struct NvmixInodeHelper
{
    /**
     * @brief 内核的 vfs inode 结构。
     */
    struct inode m_vfsInode;

    /**
     * @brief inode 对应的数据块的逻辑块号。
     * @detail 目前本文件系统最多 32 个 inode，即最多 32 个数据块，因此目前使用 unsigned short 类型。
     */
    unsigned short m_dataBlockIndex;
};


/**
 * @brief 通过有效的 vfs inode 成员获得其强绑定的 NvmixInodeHelper 结构指针。
 * @details 至于为什么是强绑定见 nvmixNewInode 的 new_inode() 代码部分。
 * @details container_of() 宏通过结构体的某个成员变量 member 的地址 ptr，反向找到整个结构体的起始地址。注意因为参数 pVfsInode 是成员的地址，所以 NvmixInodeHelper 中必须是 struct inode 而非指针 struct inode *，这样才能编译通过。
 * @details 此命名参考 EXT4_I、BTRFS_I、XFS_I 等知名文件系统。
 */
#define NVMIX_I(pVfsInode) container_of(pVfsInode, struct NvmixInodeHelper, m_vfsInode)


/**
 * @brief 在父目录中查找指定目录项。注册目录 inode 操作接口的 lookup 函数。
 * @param pParentDirInode 父目录的 inode 指针。
 * @param pDentry 要查找的目录项的 dentry 指针。
 * @param flags 标志位。
 * @return 返回 NULL 表示成功，返回非空代表可能是一些特殊情况。
 */
struct dentry *nvmixLookup(struct inode *pParentDirInode, struct dentry *pDentry, unsigned int flags);

/**
 * @brief 在父目录中创建新文件。注册目录 inode 操作接口的 create 函数。
 * @param pParentDirInode 父目录的 inode 指针。
 * @param pDentry 新创建的文件的 dentry 指针。
 * @param mode 创建模式参数。
 * @param excl 独占创建标志，若设置为 true 要求目标必须不存在。暂未用到。
 * @return 成功返回 0，失败返回非 0。
 */
int nvmixCreate(struct inode *pParentDirInode, struct dentry *pDentry, umode_t mode, bool excl);

/**
 * @brief 在父目录中删除指定文件。注册目录 inode 操作接口的 unlink 函数。
 * @param pParentDirInode 父目录的 inode 指针。
 * @param pDentry 要删除的文件的 dentry 指针。
 * @return 成功返回 0，失败返回非 0。
 */
int nvmixUnlink(struct inode *pParentDirInode, struct dentry *pDentry);

/**
 * @brief 在父目录中创建新目录。注册目录 inode 操作接口的 mkdir 函数。
 * @param pParentDirInode 父目录的 inode 指针。
 * @param pDentry 新创建的目录的 dentry 指针。
 * @param mode 创建模式参数。
 * @return 成功返回 0，失败返回非 0。
 */
int nvmixMkdir(struct inode *pParentDirInode, struct dentry *pDentry, umode_t mode);

/**
 * @brief 在父目录中删除指定目录。注册目录 inode 操作接口的 rmdir 函数。
 * @param pParentDirInode 父目录的 inode 指针。
 * @param pDentry 要删除的目录的 dentry 指针。
 * @return 成功返回 0，失败返回非 0。
 */
int nvmixRmdir(struct inode *pParentDirInode, struct dentry *pDentry);


#endif
