#include <linux/init.h>
#include <linux/module.h>
#include <linux/io.h>


MODULE_VERSION("1.0.0");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("DavidingPlus");
MODULE_DESCRIPTION("A Simple Linux Kernel Module To Test Reserved Memory.");


static void *reservedMem = NULL;
static phys_addr_t phyAddr = 0x100000000;
static unsigned long size = 0x10000000; // 256 M


static __init int reservedMemoryInit(void)
{
    int res = 0;

    unsigned int writeVal = 114514;
    unsigned int readVal1 = -1;
    unsigned int readVal2 = 0;
    int *pWrite = NULL;
    int *pRead = NULL;
    unsigned long offset = 108;


    // 映射物理内存，使用回写缓存。
    reservedMem = memremap(phyAddr, size, MEMREMAP_WB);
    if (!reservedMem)
    {
        pr_err("ReservedMemoryTest: failed to map reserved memory.\n");

        res = -EIO;
        goto ERR;
    }

    // 初始化内存。
    memset(reservedMem, 0, size);

    pr_info("ReservedMemoryTest: mapped reserved memory.\n");

    // 写入测试值。
    pWrite = (int *)((char *)reservedMem + offset);
    *pWrite = writeVal;

    // 内存屏障，保证写入完成。
    mb();

    // 读取测试值验证。
    pRead = (int *)reservedMem;
    readVal1 = *pRead;

    pr_info("ReservedMemoryTest: writeVal is %d, readVal1 is %d, writeVal == readVal1 is %d", writeVal, readVal1, writeVal == readVal1); // 114514, 0, false

    pRead = (int *)((char *)reservedMem + offset);
    readVal2 = *pRead;

    pr_info("ReservedMemoryTest: writeVal is %d, readVal2 is %d, writeVal == readVal2 is %d", writeVal, readVal2, writeVal == readVal2); // 114514, 114514, true


    pr_info("ReservedMemoryTest: module loaded successfully.\n");


ERR:
    return res;
}

static __exit void reservedMemoryExit(void)
{
    // 清空内存。
    memset(reservedMem, 0, size);

    // 释放映射。
    memunmap(reservedMem);
    reservedMem = NULL;

    pr_info("ReservedMemoryTest: unmapped reserved memory.\n");

    pr_info("ReservedMemoryTest: module unloaded successfully.\n");
}


module_init(reservedMemoryInit);
module_exit(reservedMemoryExit);
