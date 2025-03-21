/**
 * @file dir.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief dir 结构头文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_DIR_H_
#define _NVMIX_DIR_H_

#include <linux/fs.h>


int nvmixReaddir(struct file *, struct dir_context *);


#endif
