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

#include "config.h"


MODULE_VERSION(NVMIX_CONFIG_VERSION);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("DavidingPlus");
MODULE_DESCRIPTION("A Simple FileSystem for NVM and SSD Hybrid Storage");


extern struct file_system_type nvmixFileSystemType;

extern unsigned long nvmixNvmPhyAddr;

extern unsigned long nvmixNvmPhySize;


module_param(nvmixNvmPhyAddr, ulong, S_IRUGO);

module_param(nvmixNvmPhySize, ulong, S_IRUGO);


static int __init nvmixInit(void)
{
    int res = 0;


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
