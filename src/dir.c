/**
 * @file dir.c
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief dir 结构源文件。
 *
 * Copyright (c) 2024 电子科技大学 刘治学
 *
 */

#include "dir.h"

#include "defs.h"

#include <linux/kernel.h>
#include <linux/buffer_head.h>


struct file_operations nvmixDirOps = {
    .owner = THIS_MODULE,
    // read 是 2.6 版本以前的旧接口，iterate 和 iterate_shared 是 2.6 以后的接口，都可用来遍历目录。
    // read 不支持并发访问。
    // iterate 是独占式遍历，持有目录的 inode 互斥锁，支持并发访问，确保遍历期间目录结构不会被修改。
    // iterate_shared 是共享式遍历，仅持有目录的 inode 共享锁，允许其他进程并发遍历同一目录。
    // 优先使用 iterate_shared，未实现则退回 iterate。
    .read = generic_read_dir,
    .iterate_shared = nvmixReaddir,
};

int nvmixReaddir(struct file *pDir, struct dir_context *pCtx)
{
    struct inode *pInode = NULL;
    struct super_block *pSb = NULL;
    struct NvmixInodeInfo *pNii = NULL;
    struct buffer_head *pBh = NULL;
    int res = 0;


    pInode = pDir->f_inode;
    // container_of() 宏通过结构体的某个成员变量的地址，反向找到整个结构体的起始地址。
    // 注意因为是成员的地址，所以 NvmixInodeInfo 中存储的是 struct inode 而非指针，这样才能编译通过。
    pNii = container_of(pInode, struct NvmixInodeInfo, m_vfsInode);

    pSb = pInode->i_sb;
    // sb_bread() 用于从磁盘读取指定块的数据到内存缓冲区，第一个参数是超级块号，第二个参数是要读取的逻辑块号。
    pBh = sb_bread(pSb, pNii->m_dataBlock);
    if (!pBh)
    {
        pr_err("nvmixfs: could not read block.\n");

        res = -ENOMEM;
        goto ERR;
    }

    // dir_context 是内核用于目录遍历操作的关键数据结构。它封装了遍历目录时的上下文信息。主要作用是在多次调用目录遍历函数（如 .iterate 或 .iterate_shared）时，保存遍历的进度和状态，确保每次调用能正确继续上一次的位置。
    // TODO


ERR:
    return res;
}
