/**
 * @file defs.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 定义全局宏和结构体的头文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */


#ifndef _NVMIX_DEFS_H_
#define _NVMIX_DEFS_H_

#include <linux/fs.h>


/*
 * Filesystem layout:
 *
 *      SB      IZONE 	     DATA
 *    ^	    ^ (1 block)
 *    |     |
 *    +-0   +-- 4096
 * 目前本文件系统设计的非常简单，数据块以 4 KIB 为单位。第一个块存储超级块，第二个块是 inode 区。由于目前限制了目录项的数量，4 KIB 是够用的。因此数据块从块号 2 开始。这也是经典的三段式布局。
 */

#define NVMIX_BLOCK_SIZE 4096    // 数据块的大小单位，4 KIB。
#define NVMIX_MAX_ENTRY_NUM 32   // 目录下最多包含的目录项数量。
#define NVMIX_MAX_NAME_LENGTH 16 // 文件名的最大长度。
#define NVMIX_FIRST_DATA_BLOCK 2 // 起始数据块号。


struct NvmixInodeInfo
{
    struct inode m_vfsInode;

    unsigned long m_dataBlock;
};

struct NvmixDirEntry
{
    char m_name[NVMIX_MAX_NAME_LENGTH];

    unsigned long m_ino;
};


#endif
