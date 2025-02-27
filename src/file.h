/**
 * @file file.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief file 结构头文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_FILE_H_
#define _NVMIX_FILE_H_

#include <linux/fs.h>


int nvmixOpen(struct inode *, struct file *);

ssize_t nvmixRead(struct file *, char __user *, size_t, loff_t *);

ssize_t nvmixWrite(struct file *, const char __user *, size_t, loff_t *);


#endif
