/**
 * @file util.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 实用工具函数的头文件。
 * @details 此文件存放的是一些与内核无关的一些工具函数，同 defs.h 同样不能包含内核的头文件和数据结构或类型。部分内核实现的简单工具函数会在这里重新定义一遍，当然仅作代码的复制粘贴。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_UTIL_H_
#define _NVMIX_UTIL_H_

#include "globalmacros.h"
#include "defs.h"


NVMIX_EXTERN_C_BEGIN


/**
 * @brief 整除并且向上取整。
 * @param n 被除数。
 * @param d 除数。
 * @return 计算结果，向上取整的 n / d。
 * @details 此宏同内核宏 NVMIX_DIV_ROUND_UP，仅作复制粘贴。
 */
#define NVMIX_DIV_ROUND_UP(n, d) (((n) + (d)-1) / (d))


/**
 * @brief 根据文件大小计算占据 inode 中 i_blocks 的值。
 * @param size 文件大小。
 * @return 占用的磁盘块数（以 512 B 为单位）。
 * @details 通过 inode 对应文件的大小转化为 inode->i_blocks 的值，注意 inode->i_blocks 以 512 B 为单位。
 * typedef u64 blkcnt_t; typedef __u64 u64; typedef unsigned long long __u64;
 * typedef __kernel_loff_t loff_t; typedef long long __kernel_loff_t;
 * blkcnt_t（返回值） == unsigned long long
 * loff_t（参数） == long long
 */
unsigned long long nvmixCalcInodeBlocks(long long size);


NVMIX_EXTERN_C_END


#endif
