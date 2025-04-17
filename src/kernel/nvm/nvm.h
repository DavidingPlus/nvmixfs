/**
 * @file nvm.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 存放 NVM 空间变量的头文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_NVM_H_
#define _NVMIX_NVM_H_


/**
 * @brief NVM 空间的起始物理地址。
 * @details nvmixNvmPhyAddr 和 nvmixNvmPhySize 会在内核模块加载时通过模块参数配置。
 */
extern unsigned long nvmixNvmPhyAddr;

/**
 * @brief NVM 空间的大小。
 */
extern unsigned long nvmixNvmPhySize;

/**
 * @brief 映射 NVM 空间后的起始虚拟地址。
 */
extern void *nvmixNvmVirtAddr;


#endif
