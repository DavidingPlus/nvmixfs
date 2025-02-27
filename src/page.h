/**
 * @file page.h
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief page cache 结构头文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#ifndef _NVMIX_PAGE_H_
#define _NVMIX_PAGE_H_

#include <linux/fs.h>


int nvmixReadpage(struct file *, struct page *);

int nvmixWritepage(struct page *, struct writeback_control *);

int nvmixWriteBegin(struct file *, struct address_space *, loff_t, unsigned, unsigned, struct page **, void **);

int nvmixWriteEnd(struct file *, struct address_space *, loff_t, unsigned, unsigned, struct page *, void *);


#endif
