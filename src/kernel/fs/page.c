/**
 * @file page.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 页面缓存操作的源文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#include "page.h"

#include <linux/fs.h>


/**
 * @brief 注册本文件系统的页面缓存操作，提供基本的文件读写缓存能力。
 */
struct address_space_operations nvmixAops = {
    .readpage = simple_readpage,
    .write_begin = simple_write_begin,
    .write_end = simple_write_end,
};
