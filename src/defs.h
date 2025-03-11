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


/*
 * Filesystem layout:
 *
 *      SB      IZONE 	     DATA
 *    ^	    ^ (1 block)
 *    |     |
 *    +-0   +-- 4096
 * @brief 目前本文件系统设计的非常简单，数据块以 4 KIB 为单位。第一个块存储超级块，第二个块是 inode 区。由于目前限制了目录项的数量，4 KIB 是够用的。因此数据块从块号 2 开始。这也是经典的三段式布局。
 * @details 此文件定义了文件系统中必要的元数据。由于此文件 defs.h 需要被用户层程序 MkfsApp 引用，此文件中不能放置内核独有的数据结构，例如 struct inode、struct file 等，这些数据结构放到其他位置。
 */

/**
 * @brief 数据块的大小单位，4 KIB。
 */
#define NVMIX_BLOCK_SIZE 4096

/**
 * @brief 目录下最多包含的目录项数量。
 */
#define NVMIX_MAX_ENTRY_NUM 32

/**
 * @brief 文件名的最大长度。
 */
#define NVMIX_MAX_NAME_LENGTH 16

/**
 * @brief 起始数据块号。
 */
#define NVMIX_FIRST_DATA_BLOCK 2


struct NvmixDirEntry
{
    char m_name[NVMIX_MAX_NAME_LENGTH];

    unsigned long m_ino;
};


#endif
