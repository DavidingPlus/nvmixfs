/**
 * @file main.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 文件系统内核模块主程序入口文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>


MODULE_VERSION("1.0.0");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("DavidingPlus");
MODULE_DESCRIPTION("A Simple FileSystem");


static int __init nvmixfs_init(void)
{
    return 0;
}

static void __exit nvmixfs_exit(void)
{
}


module_init(nvmixfs_init);
module_exit(nvmixfs_exit);
