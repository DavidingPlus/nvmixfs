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


/**
 * @brief 处理 C/C++ 的符号兼容性。
 * @details 所有 cross-space 中的文件都会引用 globalmacros.h，因为这是个 C 库，不能保证用户态是用 C 还是 C++ 来测。C++ 的代码默认会定义 __cplusplus 宏，因此根据这个对 NVMIX_EXTERN_C_BEGIN 和 NVMIX_EXTERN_C_END 分别定义，防止符号兼容性出问题。可能想到的一种写法是将 #ifdef __cplusplus 写在 #define 中，但这样是非法的，必须使用条件编译的形式。
 */
#ifdef __cplusplus

#define NVMIX_EXTERN_C_BEGIN \
    extern "C"               \
    {

#define NVMIX_EXTERN_C_END }

#else

#define NVMIX_EXTERN_C_BEGIN

#define NVMIX_EXTERN_C_END

#endif


/**
 * @brief 定义宏 NULL。
 * @details 由于 cross-space 中的代码没有办法使用 glibc 和内核的内置宏定义，因此这里手动定义。
 * @details 本来是打算给 nvmixNvmVirtAddr 用的，但是思考语义以后觉得还是将其放在内核相关的 NVM 模块中比较合适，但是 NULL 的这个定义比较优雅，故保留。
 */
#ifndef NULL

#define NULL ((void *)0)

#endif


#endif
