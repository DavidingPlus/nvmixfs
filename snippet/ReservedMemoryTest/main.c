#include <linux/init.h>
#include <linux/module.h>
#include <linux/io.h>


MODULE_VERSION("1.0.0");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("DavidingPlus");
MODULE_DESCRIPTION("A Simple Linux Kernel Module To Test Reserved Memory.");


static void *reservedMem = NULL;


static __init int reservedMemoryInit(void)
{
    int res = 0;
    phys_addr_t physAddr = 0x0000000100000000;
    unsigned long size = 0x0000000010000000; // 256 M


    reservedMem = memremap(physAddr, size, MEMREMAP_WB);
    if (!reservedMem)
    {
        pr_err("ReservedMemoryTest: failed to map reserved memory.\n");

        res = -ENOMEM;
        goto ERR;
    }

    pr_info("ReservedMemoryTest: mapped reserved memory.\n");

    pr_info("ReservedMemoryTest: module loaded successfully.\n");


ERR:
    return res;
}

static __exit void reservedMemoryExit(void)
{
    if (reservedMem)
    {
        memunmap(reservedMem);

        pr_info("ReservedMemoryTest: unmapped reserved memory.\n");
    }


    pr_info("ReservedMemoryTest: module unloaded successfully.\n");
}


module_init(reservedMemoryInit);
module_exit(reservedMemoryExit);
