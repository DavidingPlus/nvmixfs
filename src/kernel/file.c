/**
 * @file file.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 进程打开的文件操作的源文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#include "file.h"

#include <linux/fs.h>


/**
 * @brief 由进程打开的文件（用 file 结构描述）的操作接口。
 */
struct file_operations nvmixFileFileOps = {
    .owner = THIS_MODULE,
    .open = generic_file_open,
    // 新内核优先使用 read_iter 和 write_iter 替代 read 和 write，支持异步并且更高效。
    .read_iter = generic_file_read_iter,
    .write_iter = generic_file_write_iter,
    .mmap = generic_file_mmap,
    .llseek = generic_file_llseek,
    // fsync 的作用是将文件在内存中的修改（包括数据和元数据）强制同步到物理存储设备（如磁盘），确保数据持久化。
    // 另一个命名相似的接口 fasync，用于管理文件的异步通知机制，二者完全不同。本文件系统暂不考虑。
    .fsync = generic_file_fsync,
};
