/**
 * @file globalmacros.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 全局宏定义头文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_GLOBALMACROS_H_
#define _NVMIX_GLOBALMACROS_H_


#ifdef __cplusplus

#define NVMIX_EXTERN_C_BEGIN \
    extern "C"               \
    {

#define NVMIX_EXTERN_C_END }

#else

#define NVMIX_EXTERN_C_BEGIN

#define NVMIX_EXTERN_C_END

#endif


#endif
