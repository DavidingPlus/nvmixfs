/**
 * @file page.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief page cache 结构源文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#include "page.h"

#include <linux/fs.h>


struct address_space_operations nvmixAops = {
    .readpage = simple_readpage,
    .write_begin = simple_write_begin,
    .write_end = simple_write_end,
};
