/**
 * @file defs.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 本文件系统的全局宏和数据结构的头文件。
 * @details 本文件定义了本文件系统必要的元数据，需存储在 NVM 空间上，注意与内存中 vfs 的数据结构区分开来。本文件的数据结构是本文件系统自己设计并且维护的，并且本文件需要被用户层程序 mkfs.nvmixfs 引用，因此文件中不能放置任何内核 vfs 的数据结构或类型，例如 struct inode、struct buffer_head 等。但在实际过程中本文件系统的元数据结构显然是需要与 vfs 的数据结构打交道的，因此设计了辅助结构 NvmixNvmHelper 和 NvmixInodeHelper 等。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_DEFS_H_
#define _NVMIX_DEFS_H_

#include "globalmacros.h"


NVMIX_EXTERN_C_BEGIN


/**
 * @brief 文件系统的魔数。
 * @details 此十六进制数是 nvmix 的 ASCII 编码表示，用于唯一标识本文件系统。
 */
#define NVMIX_MAGIC_NUMBER 0x6E766d6978

/**
 * @brief 根目录的 inode 号。
 */
#define NVMIX_ROOT_DIR_INODE_NUMBER 0

/**
 * @brief 数据块的大小单位，4 KIB。
 */
#define NVMIX_BLOCK_SIZE 4096

/**
 * @brief 超级块区在 NVM 空间上的偏移量。
 */
#define NVMIX_SUPER_BLOCK_OFFSET 0

/**
 * @brief inode 区在 NVM 空间上的偏移量。
 */
#define NVMIX_INODE_BLOCK_OFFSET 1 * NVMIX_BLOCK_SIZE

/**
 * @brief 起始数据块的逻辑块号。
 */
#define NVMIX_FIRST_DATA_BLOCK_INDEX 0

/**
 * @brief 文件系统最多的 inode 数量。
 * @todo 目前先设置为 32，后续增加并完善设计。
 */
#define NVMIX_MAX_INODE_NUM 32

/**
 * @brief 目录下最多包含的目录项数量。
 * @details 注意，此项与 NVMIX_MAX_INODE_NUM 并不是一个东西。NVMIX_MAX_INODE_NUM 是文件系统总 inode 的数量，NVMIX_MAX_ENTRY_NUM 是一个目录下最多包含的目录项数量。从定义可知，NVMIX_MAX_ENTRY_NUM 应小于等于 NVMIX_MAX_INODE_NUM。
 */
#define NVMIX_MAX_ENTRY_NUM 32

/**
 * @brief 文件名的最大长度。
 */
#define NVMIX_MAX_NAME_LENGTH 16


/**
 * @struct NvmixVersion
 * @brief 描述文件系统的版本号。
 */
struct NvmixVersion
{
    /**
     * @brief 主版本号。
     * @details unsigned char 类型是 1 个字节，8 位。能表示的数的范围是 0 到 255（2 的 8 次方 - 1）。表示版本完全够用。
     */
    unsigned char m_major;

    /**
     * @brief 次版本号。
     */
    unsigned char m_minor;

    /**
     * @brief 修订版本号。
     */
    unsigned char m_alter;
};

/**
 * @struct NvmixSuperBlock
 * @brief 文件系统超级块的元数据信息。
 * @todo 目前 NvmixSuperBlock 结构体占据的空间远小于 4 KIB，但仍需要使用一个 4 KIB 的逻辑块（文件系统必须以一定大小的块为单位），可能存在空间浪费的问题。目前暂不做优化。未使用空间用作保留，便于未来扩展。另外迁移到 NVM 空间以后，其实是不需要考虑以块为单位的问题的，但目前为了简便，仍然按照块划分的形式处理，后续做内存分配的设计。
 */
struct NvmixSuperBlock
{
    /**
     * @brief 文件系统的标识魔数。
     * @details 用于标识文件系统类型。加载文件系统时需校验此值，防止误挂载其他文件系统，如 EXT4 魔数为 0xEF53。
     */
    unsigned long m_magic;

    /**
     * @brief 管理 inode 分配状态的位图信息。
     * @details unsigned long 类型是 8 个字节，64 位。囊括了 NVMIX_MAX_INODE_NUM 的 32 位。低 32 位的每一位代表一个 inode 的分配信息。
     */
    unsigned long m_imap;

    /**
     * @brief 文件系统的版本号。
     */
    struct NvmixVersion m_version;
};

/**
 * @struct NvmixInode
 * @brief 文件系统 inode 的元数据信息。
 * @details 每个 inode 都有一个 NvmixInode 结构。NVM 空间上 inode 区存储的就是 NvmixInode[] 数组。
 */
struct NvmixInode
{
    /**
     * @brief 文件类型和权限的位掩码组合。
     * @details 高位表示文件类型，低位表示权限。
     * 文件类型：使用宏定义判断（如 S_ISREG(mode) 检测普通文件）。
     * - S_IFMT：文件类型掩码（0170000）。
     * - S_IFREG：普通文件（0100000）。
     * - S_IFDIR：目录（0040000）。
     * 权限部分：低 12 位，按 0ABC 八进制格式解析。
     * - A（用户）：0 ~ 2 位 == rwx （例: 07 == 111₂ == rwx）。
     * - B（组）：3 ~ 5 位 == rwx （例: 05 == 101₂ == r-x）。
     * - C（其他）：6 ~ 8 位 == rwx （例: 04 == 100₂ == r--）。
     */
    unsigned int m_mode;

    /**
     * @brief 文件或目录所有者的用户 ID。
     */
    unsigned int m_uid;

    /**
     * @brief 文件或目录所属组的组 ID。
     */
    unsigned int m_gid;

    /**
     * @brief 文件或目录的大小（以字节为单位）。
     */
    unsigned int m_size;

    /**
     * @brief 文件或目录对应的数据块的逻辑块号。
     */
    unsigned short m_dataBlockIndex;
};

/**
 * @struct NvmixDentry
 * @brief 目录对应的数据块的各条目录项的信息。
 * @details 目录项指目录下的文件或子目录（包括 . 和 ..）。本结构类似 vfs 的 dentry 结构的作用，将 inode 和目录项的名字关联在一起。目录的数据块存储的就是 NvmixDentry[] 数组，记录该目录下所有目录项的信息。
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


NVMIX_EXTERN_C_END


#endif
