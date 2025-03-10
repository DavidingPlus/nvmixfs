/**
 * @file inode.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief inode 结构头文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_INODE_H_
#define _NVMIX_INODE_H_

#include <linux/fs.h>


struct dentry *nvmixLookup(struct inode *, struct dentry *, unsigned int);

int nvmixCreate(struct inode *, struct dentry *, umode_t, bool);


#endif
