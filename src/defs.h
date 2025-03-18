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
 * @brief 目前本文件系统设计的非常简单，数据块以 4 KIB 为单位。第一个块存储超级块，第二个块是 inode 区。由于目前限制了文件系统总 inode 的数量为 32，4 KIB 是够用的。因此数据块从块号 2 开始。这是经典的三段式布局。
 * @details 此文件定义了文件系统中必要的元数据。由于此文件 defs.h 需要被用户层程序 MkfsApp 引用，此文件中不能放置内核独有的数据结构，例如 struct inode、struct file 等，这些数据结构放到其他位置。
 */

/**
 * @brief 超级块的逻辑块号。
 * @details 逻辑块号的值是相对于文件系统的起始位置，下面的 inode 和起始数据块同理。
 */
#define NVMIX_SUPER_BLOCK_INDEX 0

/**
 * @brief inode 区的逻辑块号。
 */
#define NVMIX_INODE_BLOCK_INDEX 1

/**
 * @brief 起始数据块的逻辑块号。
 */
#define NVMIX_FIRST_DATA_BLOCK_INDEX 2

/**
 * @brief 数据块的大小单位，4 KIB。
 */
#define NVMIX_BLOCK_SIZE 4096

/**
 * @brief 文件系统最多的 inode 数量。
 * @todo 目前先设置为 32，后续增加并修正设计和设计。
 */
#define NVMIX_MAX_INODE_NUM 32

/**
 * @brief 目录下最多包含的目录项数量。
 * @details 注意，此项与 NVMIX_MAX_INODE_NUM 并不是一个东西。NVMIX_MAX_INODE_NUM 是文件系统总 inode 的数量，NVMIX_MAX_ENTRY_NUM 是一个目录下最多包含的目录项数量。NVMIX_MAX_ENTRY_NUM 应小于等于 NVMIX_MAX_INODE_NUM。
 */
#define NVMIX_MAX_ENTRY_NUM 32

/**
 * @brief 文件名的最大长度。
 */
#define NVMIX_MAX_NAME_LENGTH 16


struct NvmixDentry
{
    char m_name[NVMIX_MAX_NAME_LENGTH];

    unsigned long m_ino;
};

struct NvmixInode
{
    unsigned int m_mode;

    unsigned int m_uid;

    unsigned int m_gid;

    unsigned int m_size;

    unsigned long m_dataBlockIndex;
};


#endif
