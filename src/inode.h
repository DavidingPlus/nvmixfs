/**
 * @file inode.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief inode 结构头文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_INODE_H_
#define _NVMIX_INODE_H_

#include <linux/fs.h>


/**
 * @struct NvmixInodeInfo
 * @brief 本文件系统维护的 inode 元数据信息。
 */
struct NvmixInodeInfo
{
    /**
     * @brief 内核的 vfs inode 结构。
     */
    struct inode m_vfsInode;

    /**
     * @brief inode 对应的数据块的逻辑块号。
     */
    unsigned long m_dataBlockIndex;
};


/**
 * @brief 通过有效的 vfs inode 获得本文件系统维护的 NvmixInodeInfo 结构指针。
 * @details container_of() 宏通过结构体的某个成员变量 member 的地址 ptr，反向找到整个结构体的起始地址。注意因为参数 pVfsInode 是成员的地址，所以 NvmixInodeInfo 中必须是 struct inode 而非指针 struct inode *，这样才能编译通过。
 * @details 此命名参考 EXT4_I、BTRFS_I、XFS_I 等知名文件系统。
 */
#define NVMIX_I(pVfsInode) container_of(pVfsInode, struct NvmixInodeInfo, m_vfsInode)


struct dentry *nvmixLookup(struct inode *, struct dentry *, unsigned int);

int nvmixCreate(struct inode *, struct dentry *, umode_t, bool);


#endif
