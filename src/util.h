/**
 * @file util.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 存放实用工具函数的头文件。
 * @details 此文件存放的是一些与内核无关的一些工具函数，因此同 defs.h 同样不能包含内核的头文件和数据结构或类型。部分内核实现的简单工具函数会在这里重新定义一遍，仅作代码的复制粘贴。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_UTIL_H_
#define _NVMIX_UTIL_H_

#include "defs.h"


#define NVMIX_DIV_ROUND_UP(n, d) (((n) + (d)-1) / (d))

// 通过 inode 对应目录项的大小转化为 inode->i_blocks 的值，注意 inode->i_blocks 以 512 B 为单位。
// typedef u64 blkcnt_t; typedef __u64 u64; typedef unsigned long long __u64;
// typedef __kernel_loff_t loff_t; typedef long long __kernel_loff_t;
// blkcnt_t（返回值） == unsigned long long
// loff_t（参数） == long long
unsigned long long nvmixCalcInodeBlocks(long long size);


#endif
