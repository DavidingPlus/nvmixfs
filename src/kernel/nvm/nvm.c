/**
 * @file nvm.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 存放 NVM 空间的变量的源文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#include "nvm.h"

#include <linux/stddef.h>


unsigned long nvmixNvmPhyAddr = 0;

unsigned long nvmixNvmPhySize = 0;

void *nvmixNvmVirtAddr = NULL;
