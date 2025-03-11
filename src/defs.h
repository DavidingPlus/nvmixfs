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


#define NVMIX_BLOCK_SIZE 4096    // 数据块的单位，4 KIB。
#define NVMIX_MAX_ENTRY_NUM 32   // 目录项最多包含的数量。
#define NVMIX_MAX_NAME_LENGTH 16 // 文件名的最大长度。


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
