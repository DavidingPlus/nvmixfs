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
    .unlink = nvmixUnlink,
    .mkdir = nvmixMkdir,
    .rmdir = nvmixRmdir,
    .rename = nvmixRename,
    .link = nvmixLink,
    .symlink = nvmixSymlink,
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

int nvmixUnlink(struct inode *pDir, struct dentry *pDentry)
{
    // TODO

    return 0;
}

int nvmixMkdir(struct inode *pDir, struct dentry *pDentry, umode_t mode)
{
    // TODO

    return 0;
}

int nvmixRmdir(struct inode *pDir, struct dentry *pDentry)
{
    // TODO

    return 0;
}

int nvmixRename(struct inode *pOldDir, struct dentry *pOldDentry, struct inode *pNewDir, struct dentry *pNewDentry, unsigned int flags)
{
    // TODO

    return 0;
}

int nvmixLink(struct dentry *pOldDentry, struct inode *pDir, struct dentry *pDentry)
{
    // TODO

    return 0;
}

int nvmixSymlink(struct inode *pDir, struct dentry *pDentry, const char *pSymname)
{
    // TODO

    return 0;
}
