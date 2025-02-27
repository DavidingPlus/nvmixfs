/**
 * @file file.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief file 结构源文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#include "file.h"


struct file_operations nvmixFileOps = {
    .owner = THIS_MODULE,
    .open = nvmixOpen,
    // 新内核优先使用 read_iter 和 write_iter，支持异步并且更高效。这里先暂时使用老的 read 和 write。后续依据需求看是否迁移。
    .read = nvmixRead,
    .write = nvmixWrite,
    .llseek = generic_file_llseek,
    .fsync = generic_file_fsync,
};


int nvmixOpen(struct inode *pInode, struct file *pFile)
{
    // TODO

    return 0;
}

ssize_t nvmixRead(struct file *pFile, char __user *pBuf, size_t len, loff_t *pPos)
{
    // TODO

    return (ssize_t)0;
}

ssize_t nvmixWrite(struct file *pFile, const char __user *pBuf, size_t len, loff_t *pPos)
{
    // TODO

    return (ssize_t)0;
}
