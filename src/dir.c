/**
 * @file dir.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief dir 结构源文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#include "dir.h"


struct file_operations nvmixDirOps = {
    .owner = THIS_MODULE,
    .read = generic_read_dir,
    // iterate 是独占式遍历，持有目录的 inode 互斥锁，确保遍历期间目录结构不会被修改。
    // iterate_shared 是共享式遍历，仅持有目录的 inode 共享锁，允许其他进程并发遍历同一目录。
    // 优先使用 iterate_shared，未实现则退回 iterate。
    .iterate_shared = nvmixReaddir,
};


int nvmixReaddir(struct file *pDir, struct dir_context *pCtx)
{
    // TODO

    return 0;
}
