/**
 * @file main.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 内核模块主程序入口文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/io.h>

#include "config.h"


MODULE_VERSION(NVMIX_CONFIG_VERSION);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("DavidingPlus");
MODULE_DESCRIPTION("A Simple FileSystem for NVM and SSD Hybrid Storage.");


extern struct file_system_type nvmixFileSystemType;

extern unsigned long nvmixNvmPhyAddr;

extern unsigned long nvmixNvmPhySize;

extern void *nvmixNvmVirtAddr;


/**
 * @brief 内核提供的用于定义内核模块参数的宏。
 * @param name 参数名称。
 * @param type 参数类型。
 * @param perm 权限标志。
 * @details 参数类型是下列之一：short, ushort, int, uint, long, ulong, charp, bool。
 * @details 权限标志就是用户、组和其他是否可读、可写和可执行（八进制数）。常用组合：S_IRUGO 用户、组、其他均可读（S_IRUSR | S_IRGRP | S_IROTH），S_IWUSR 仅用户可写，S_IRUGO | S_IWUSR 所有人可读，仅用户可写。
 */
module_param(nvmixNvmPhyAddr, ulong, S_IRUGO);
MODULE_PARM_DESC(nvmixNvmPhyAddr, "Starting Physical Address Of NVM Space.");

module_param(nvmixNvmPhySize, ulong, S_IRUGO);
MODULE_PARM_DESC(nvmixNvmPhySize, "Size Of NVM Space.");


static int __init nvmixInit(void)
{
    int res = 0;


    // 映射物理内存，使用回写缓存。
    // ioremap()：用于映射 I/O 内存（如设备寄存器、硬件缓冲区等）到内核虚拟地址空间。这些区域通常是非缓存（Uncached）的，以确保对设备的直接访问。
    // 在计算机系统中，I/O 内存是指通过内存映射 I/O（Memory-Mapped I/O, MMIO）方式访问的硬件设备资源。这些资源可以是设备的寄存器、缓冲区或其他控制接口，它们被映射到处理器的物理地址空间中，使得软件（如操作系统或驱动程序）能够像访问普通内存一样读写这些硬件资源。
    // memremap()：用于映射普通内存（如 RAM、持久内存等）到内核虚拟地址空间。支持灵活的缓存策略（如 Write-Through、Write-Back）。
    // memremap() 的第三个参数指定缓存类型：MEMREMAP_WB Write-Back 缓存（性能优化）。MEMREMAP_WT Write-Through 缓存（写入直达内存）。MEMREMAP_UC Uncached（类似 ioremap）。
    nvmixNvmVirtAddr = memremap(nvmixNvmPhyAddr, nvmixNvmPhySize, MEMREMAP_WB);
    if (IS_ERR(nvmixNvmVirtAddr))
    {
        pr_err("nvmixfs: failed to map reserved memory.\n");

        res = -EIO;
        goto ERR;
    }

    // 初始化内存。
    memset(nvmixNvmVirtAddr, 0, nvmixNvmPhySize);

    pr_info("nvmixfs: mapped reserved memory successfully.\n");

    // 注册文件系统。
    res = register_filesystem(&nvmixFileSystemType);
    if (0 != res)
    {
        pr_err("nvmixfs: failed to register nvmixfs.\n");

        goto ERR;
    }

    pr_info("nvmisfs: nvmixfs module loaded.\n");


ERR:
    return res;
}

static void __exit nvmixExit(void)
{
    int res = 0;


    // 清空内存。
    memset(nvmixNvmVirtAddr, 0, nvmixNvmPhySize);

    // 释放映射。
    memunmap(nvmixNvmVirtAddr);
    nvmixNvmVirtAddr = NULL;

    pr_info("nvmixfs: unmapped reserved memory successfully.\n");

    // 注销文件系统。
    res = unregister_filesystem(&nvmixFileSystemType);
    if (0 != res)
    {
        pr_err("nvmixfs: failed to unregister nvmixfs.\n");


        return;
    }

    pr_info("nvmisfs: nvmixfs module unloaded.\n");
}


module_init(nvmixInit);
module_exit(nvmixExit);
