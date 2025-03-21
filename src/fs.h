/**
 * @file fs.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief file_system_type 结构头文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_FS_H_
#define _NVMIX_FS_H_

#include <linux/fs.h>
#include <linux/buffer_head.h>


struct NvmixSuperBlockHelper
{
    struct buffer_head *m_pBh;

    /**
     * @brief 管理 inode 分配状态的位图信息。
     * @details unsigned long 类型 32 位，与 NVMIX_MAX_INODE_NUM（32）刚好对应。每一位代表一个 inode 的分配信息。
     */
    unsigned long m_imap;

    unsigned char m_version;
};


struct dentry *nvmixMount(struct file_system_type *, int, const char *, void *);

void nvmixKillSb(struct super_block *);

int nvmixFillSuper(struct super_block *, void *, int);

void nvmixPutSuper(struct super_block *);

struct inode *nvmixAllocInode(struct super_block *);

void nvmixDestroyInode(struct inode *);

int nvmixWriteInode(struct inode *, struct writeback_control *);

struct inode *nvmixIget(struct super_block *, unsigned long);

// 通过 inode 对应目录项的大小转化为 inode->i_blocks 的值，注意 inode->i_blocks 以 512 B 为单位。
// TODO 这个函数只是一个工具函数，按理来讲不应该放在头文件中暴露给用户，而应该在源文件中定义为 static。但我想在用户态进行单元测试，目前必须暴露出来。后续思考如何更好的解决这个问题。
blkcnt_t nvmixCalcInodeBlocks(loff_t);


#endif
