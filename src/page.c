/**
 * @file page.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief page cache 结构源文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#include "page.h"


struct address_space_operations nvmixAops = {
    .readpage = nvmixReadpage,
    .writepage = nvmixWritepage,
    .write_begin = nvmixWriteBegin,
    .write_end = nvmixWriteEnd,
};


int nvmixReadpage(struct file *pFile, struct page *pPage)
{
    // TODO

    return 0;
}

int nvmixWritepage(struct page *pPage, struct writeback_control *pWbc)
{
    // TODO

    return 0;
}

int nvmixWriteBegin(struct file *pFile, struct address_space *pMapping, loff_t pos, unsigned len, unsigned flags, struct page **ppPage, void **ppFsdata)
{
    // TODO

    return 0;
}

int nvmixWriteEnd(struct file *pFile, struct address_space *pMapping, loff_t pos, unsigned len, unsigned copied, struct page *pPage, void *pFsdata)
{
    // TODO

    return 0;
}
