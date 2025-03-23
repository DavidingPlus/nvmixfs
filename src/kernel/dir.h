/**
 * @file dir.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 进程打开的目录操作的头文件。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_DIR_H_
#define _NVMIX_DIR_H_

#include <linux/fs.h>


/**
 * @brief 注册 file_operations 的 iterate 函数，用于遍历当前目录的目录项。
 * @param pDirFile 进程打开的目录的 file 指针。
 * @param pCtx 存储遍历的目录项，由内核提供维护。
 * @return 是否成功。0 代表成功，非 0 代表失败。
 */
int nvmixReaddir(struct file *pDirFile, struct dir_context *pCtx);


#endif
