#include <linux/init.h>
#include <linux/module.h>
#include <linux/io.h>
#include <asm/cacheflush.h>


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
    if (IS_ERR(reservedMem))
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

    /*
     * 调用 clflush_cache_range 刷新指定地址的 CPU 缓存：
     * - addr: 要刷新的虚拟地址（需确保在映射范围内）。
     * - size: 刷新的字节数（实际会按缓存行对齐扩展范围）。
     *
     * 函数内部已通过 mb() 屏障确保：
     * 1. 刷新前所有内存操作已完成（防止重排到刷新前）。
     * 2. 刷新操作本身顺序性（CLFLUSHOPT 是弱序指令）。
     * 3. 刷新后屏障保证后续操作不会重排到刷新前。
     *
     * 适用于需确保数据从 CPU 缓存刷回物理内存的场景（如持久化内存操作）。
     * 注意：地址 addr 应尽量对齐到缓存行（64 字节）以提升性能。
     * void clflush_cache_range(void *vaddr, unsigned int size)
     * {
     *     mb();
     *     clflush_cache_range_opt(vaddr, size);
     *     mb();
     * }
     * EXPORT_SYMBOL_GPL(clflush_cache_range);
     *
     * 与用户层函数 msync() 不同，clflush_cache_range() 不需要页对齐。
     */
    clflush_cache_range(pWrite, sizeof(int));

    // 这两句是老代码，可以注释掉了。

    // 将数据将 CPU 缓存刷回内存。
    // clflush：将缓存行从所有层级的缓存中逐出并写回内存，之后该缓存行标记为无效。缓存行被无效化（后续访问需重新加载）。
    // clwb：将缓存行写回内存，但保留缓存行有效（保持数据在缓存中）。缓存行保持有效（可被后续访问复用）。
    // clflush(pWrite);

    // 内存屏障，保证写入完成。
    // wmb：写操作顺序。确保屏障之前的所有写操作在屏障之后的写操作之前完成。
    // rmb：读操作顺序。确保屏障之前的所有读操作在屏障之后的读操作之前完成。
    // mb：读写操作顺序。确保屏障之前的所有读写操作在屏障之后的读写操作之前完成。
    // wmb();

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
