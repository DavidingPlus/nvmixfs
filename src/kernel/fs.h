/**
 * @file fs.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief file_system_type 结构头文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_FS_H_
#define _NVMIX_FS_H_

#include "defs.h"

#include <linux/fs.h>
#include <linux/buffer_head.h>


struct NvmixSuperBlockHelper
{
    /**
     * @brief 磁盘上超级块区的缓冲区指针。
     * @details 这个指针在 fill_super() 函数中被初始化，然后就没有释放过。记录这个指针是为了在操作文件系统的时候能够及时更新超级块区的信息。这是不可避免的。
     */
    struct buffer_head *m_pBh;

    /**
     * @brief 管理 inode 分配状态的位图信息。
     * @details 同 defs.h 中 NvmixSuperBlock 的 m_imap。
     */
    unsigned long m_imap;

    /**
     * @brief 文件系统的版本号。
     */
    struct NvmixVersion m_version;
};


struct dentry *nvmixMount(struct file_system_type *, int, const char *, void *);

void nvmixKillSb(struct super_block *);

int nvmixFillSuper(struct super_block *, void *, int);

void nvmixPutSuper(struct super_block *);

struct inode *nvmixAllocInode(struct super_block *);

void nvmixDestroyInode(struct inode *);

int nvmixWriteInode(struct inode *, struct writeback_control *);

struct inode *nvmixIget(struct super_block *, unsigned long);


#endif
