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
MODULE_DESCRIPTION("A Simple FileSystem for NVM and SSD Hybrid Storage");


extern struct file_system_type nvmixFileSystemType;

extern unsigned long nvmixNvmPhyAddr;

extern unsigned long nvmixNvmPhySize;

extern void *nvmixNvmVirtAddr;


module_param(nvmixNvmPhyAddr, ulong, S_IRUGO);

module_param(nvmixNvmPhySize, ulong, S_IRUGO);


static int __init nvmixInit(void)
{
    int res = 0;


    // 映射物理内存，使用回写缓存。
    nvmixNvmVirtAddr = memremap(nvmixNvmPhyAddr, nvmixNvmPhySize, MEMREMAP_WB);
    if (!nvmixNvmVirtAddr)
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
