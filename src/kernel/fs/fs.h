/**
 * @file fs.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 文件系统类型结构和超级块操作头文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_FS_H_
#define _NVMIX_FS_H_

#include "defs.h"

#include <linux/fs.h>
#include <linux/buffer_head.h>


/**
 * @struct NvmixNvmHelper
 * @brief 辅助结构，存储 NVM 空间超级块和 inode 区的映射虚拟起始地址。
 */
struct NvmixNvmHelper
{
    /**
     * @brief NVM 空间上超级块的起始虚拟地址。
     */
    void *m_superBlockVirtAddr;

    /**
     * @brief NVM 空间上 inode 区的起始虚拟地址。
     */
    void *m_inodeVirtAddr;
};


/**
 * @brief 挂载文件系统实例。注册文件系统类型结构的 mount 函数。
 * @param pFileSystemType 要挂载的文件系统类型描述符的指针。
 * @param flags 挂载标志位。
 * @param pDevName 要挂载的设备路径。
 * @param pData 挂载时的额外选项。
 * @return 成功返回根目录的 dentry 指针，失败返回 NULL。
 */
struct dentry *nvmixMount(struct file_system_type *pFileSystemType, int flags, const char *pDevName, void *pData);

/**
 * @brief 销毁文件系统超级块及其关联资源。注册文件系统类型结构的 kill_sb 函数。
 * @param pSb 超级块指针。
 */
void nvmixKillSb(struct super_block *pSb);

/**
 * @brief 初始化文件系统超级块。
 * @param pSb 超级块指针。
 * @param pData 挂载时的额外选项。暂未使用。
 * @param silent 静默模式标志。
 * @return 成功返回 0，失败返回非 0。
 * @details fill_super() 是 Linux 内核文件系统模块中用于初始化超级块的核心函数，用于将磁盘上的我们设计的文件系统元数据加载到内存中，并建立文件系统的基本结构，使内核能够识别和管理该文件系统。
 * @details 在实现里，我发现元数据都是从磁盘上读取来的，并没有发现第一次初始化数据的过程，这是为什么呢？明确 fill_super() 的语义是将磁盘上我们设计的文件系统元数据加载并初始化文件系统，它不负责第一次初始化磁盘上数据的过程。整个文件系统除了这个内核模块还有 mkfs.nvmixfs 这个用户层程序，它负责格式化文件系统并第一次初始化数据。
 */
int nvmixFillSuper(struct super_block *pSb, void *pData, int silent);

/**
 * @brief 释放超级块持有的资源。注册超级块操作的 put_super 函数。
 * @param pSb 超级块指针。
 * @details put_super 的作用是在文件系统卸载或不再需要超级块时，释放与该超级块关联的资源。
 */
void nvmixPutSuper(struct super_block *pSb);

/**
 * @brief 分配并初始化 vfs inode。注册超级块操作的 alloc_inode 函数。
 * @param pSb 超级块指针。
 * @return
 */
struct inode *nvmixAllocInode(struct super_block *pSb);

/**
 * @brief 销毁 vfs inode。注册超级块操作的 destroy_inode 函数。
 * @param pInode 要删除的 inode 指针。
 */
void nvmixDestroyInode(struct inode *pInode);

/**
 * @brief 将内存中的 vfs inode 数据持久化到盘上的 NvmixInode 元数据。注册超级块操作的 write_inode 函数。
 * @param pInode vfs inode 指针。
 * @param pWbc 回写控制参数及上下文信息。
 * @return
 */
int nvmixWriteInode(struct inode *pInode, struct writeback_control *pWbc);

/**
 * @brief 通过超级块和全局唯一 inode 号获得 inode 指针。
 * @param pSb 超级块指针。
 * @param ino 全局唯一 inode 号。
 * @return 成功返回 inode 指针，失败返回 NULL。
 * @details 此函数包括如下两种情况：
 * 1. 从缓存中获取 inode：若 inode 已存在于内存中（缓存命中），直接返回。
 * 2. 初始化新 inode：若 inode 未缓存（I_NEW 状态），从磁盘读取元数据并初始化 vfs inode。
 */
struct inode *nvmixIget(struct super_block *pSb, unsigned long ino);


#endif
