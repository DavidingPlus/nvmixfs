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

int nvmixUnlink(struct inode *, struct dentry *);

int nvmixMkdir(struct inode *, struct dentry *, umode_t);

int nvmixRmdir(struct inode *, struct dentry *);

int nvmixRename(struct inode *, struct dentry *, struct inode *, struct dentry *, unsigned int);

int nvmixLink(struct dentry *, struct inode *, struct dentry *);

int nvmixSymlink(struct inode *, struct dentry *, const char *);


#endif
