/**
 * @file fs.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief file_system_type 结构头文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_FS_H_
#define _NVMIX_FS_H_

#include <linux/fs.h>


struct dentry *nvmixMount(struct file_system_type *, int, const char *, void *);

void nvmixKillSb(struct super_block *);


#endif
