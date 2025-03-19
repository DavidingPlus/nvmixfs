/**
 * @file defs.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 定义本文件系统的元数据的头文件。
 * @details 本文件定义了本文件系统必要的元数据，需要存储在磁盘上。注意与内存中 vfs 的数据结构区分开来，此文件的数据结构是本文件系统自己定义并且维护的。本文件需要被用户层程序 MkfsApp 引用，因此文件中不能放置任何内核 vfs 的数据结构，例如 struct inode、struct buffer_head 等。但在实际过程中本文件系统的元数据结构显然是需要与 vfs 的数据结构打交道的，因此设计了辅助结构 NvmixSuperBlockHelper 和 NvmixInodeHelper 等。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */


#ifndef _NVMIX_DEFS_H_
#define _NVMIX_DEFS_H_


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


/**
 * @struct NvmixDentry
 * @brief 目录项的元数据信息。
 * @details 目录项指目录下的文件或子目录（包括 . 和 ..）。本结构类似 vfs 的 dentry 结构的作用，将 inode 和 目录项的名字关联在一起。
 */
struct NvmixDentry
{
    /**
     * @brief 目录项的名字。
     */
    char m_name[NVMIX_MAX_NAME_LENGTH];

    /**
     * @brief 目录项在 vfs 中全局唯一的 inode 号。
     */
    unsigned long m_ino;
};

/**
 * @struct NvmixInode
 * @brief inode 的元数据信息。
 */
struct NvmixInode
{
    unsigned int m_mode;

    unsigned int m_uid;

    unsigned int m_gid;

    unsigned int m_size;

    unsigned short m_dataBlockIndex;
};


#endif
