/**
 * @file util.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 存放实用工具函数的源文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#include "util.h"


unsigned long long nvmixCalcInodeBlocks(long long size)
{
    if (0 == size) return 0;


    return NVMIX_DIV_ROUND_UP(size, NVMIX_BLOCK_SIZE) * (NVMIX_BLOCK_SIZE / 512);
}
