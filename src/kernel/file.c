/**
 * @file file.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief file 结构源文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#include "file.h"

#include <linux/fs.h>


// 由进程打开的文件（用 file 结构描述）的操作接口。
struct file_operations nvmixFileFileOps = {
    .owner = THIS_MODULE,
    .open = generic_file_open,
    // 新内核优先使用 read_iter 和 write_iter 替代 read 和 write，支持异步并且更高效。
    .read_iter = generic_file_read_iter,
    .write_iter = generic_file_write_iter,
    .mmap = generic_file_mmap,
    .llseek = generic_file_llseek,
    .fsync = generic_file_fsync,
};
