/**
 * @file inode.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief inode 结构源文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#include "inode.h"


struct inode_operations nvmixInodeOps = {
    .lookup = nvmixLookup,
    .create = nvmixCreate,
};

struct dentry *nvmixLookup(struct inode *pDir, struct dentry *pDentry, unsigned int flags)
{
    // TODO

    return NULL;
}

int nvmixCreate(struct inode *pDir, struct dentry *pDentry, umode_t mode, bool excl)
{
    // TODO

    return 0;
}
