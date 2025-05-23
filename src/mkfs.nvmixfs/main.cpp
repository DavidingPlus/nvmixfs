/**
 * @file main.cpp
 * @author DavidingPlus (davidingplus@qq.com)
 * @brief 格式化 nvmixfs 文件系统的用户层程序。
 *
 * Copyright (c) 2025 电子科技大学 刘治学
 *
 */

#include <iostream>
#include <cstring>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "config.h"

#include "defs.h"


int main(int argc, char const *argv[])
{
    if (4 != argc)
    {
        std::cerr << "Error: Invalid arguments.\n"
                  << "Usage: " << argv[0]
                  << " <nvm-device-path> <nvm-size-bytes> <ssd-device-path>\n"
                  << "  <nvm-device-path>     Path to persistent memory device (e.g. /dev/pmem0)\n"
                  << "  <nvm-size-bytes> Size of NVM space in bytes (e.g. 1048576 for 1MB)\n"
                  << "  <ssd-device-path>     Path to SSD block device (e.g. /dev/sdb2)\n";


        return EXIT_FAILURE;
    }


    const char *nvmDevicePath = argv[1];
    const char *ssdDevicePath = argv[3];
    unsigned long nvmPhySize = 0;

    // 将字符串表示的十六进制数字转化为 unsigned long 类型的十进制数。
    try
    {
        nvmPhySize = std::stoi(argv[2], nullptr, 16);
    }
    catch (const std::exception &e)
    {
        throw std::invalid_argument(std::string("Invalid argument: ") + argv[2]);
    }


    // 写入元数据。
    int nvmFd = open(nvmDevicePath, O_RDWR);
    if (-1 == nvmFd)
    {
        perror("open");


        return EXIT_FAILURE;
    }

    // 具体映射逻辑见 snippet/PmemTest/main.cpp，使用 mmap 映射到用户态内存中，并通过 msync() 保证同步。
    void *nvmVirtAddr = mmap(NULL, nvmPhySize, PROT_READ | PROT_WRITE, MAP_SHARED, nvmFd, 0);
    if (MAP_FAILED == nvmVirtAddr)
    {
        perror("mmap");

        close(nvmFd);


        return EXIT_FAILURE;
    }

    NvmixSuperBlock superBlock = {
        .m_magic = NVMIX_MAGIC_NUMBER,
        // 直接访问块设备就不会走 vfs 这一层了，所以初始化的时候 m_imap 需要考虑 reserved.txt（为了测试预先保留在本文件系统中的文件），写为 3 而不是 1。
        .m_imap = 0x03,
        .m_version = NvmixVersion{
            .m_major = NVMIX_CONFIG_VERSION_MAJOR,
            .m_minor = NVMIX_CONFIG_VERSION_MINOR,
            .m_alter = NVMIX_CONFIG_VERSION_ALTER,
        },
    };

    NvmixSuperBlock *superBlockVirtAddr = (NvmixSuperBlock *)((char *)nvmVirtAddr + NVMIX_SUPER_BLOCK_OFFSET);

    *superBlockVirtAddr = superBlock;

    // msync() 是一个用户态函数，用于将通过 mmap() 映射的内存区域中的修改数据强制同步回磁盘或其他底层存储设备。
    // 写操作在用户层通过 msync 同步，在内核层通过 clflush_cache_range 同步。
    int res = msync(superBlockVirtAddr, sizeof(NvmixSuperBlock), MS_SYNC);
    if (-1 == res)
    {
        perror("msync");


        return EXIT_FAILURE;
    }

    NvmixInode rootDirInode = {
        .m_mode = S_IFDIR | 0755,
        .m_uid = 0,
        .m_gid = 0,
        .m_size = 0,
        .m_dataBlockIndex = NVMIX_FIRST_DATA_BLOCK_INDEX,
    };

    NvmixInode fileInode = {
        .m_mode = S_IFREG | 0664,
        .m_uid = 0,
        .m_gid = 0,
        .m_size = 0,
        .m_dataBlockIndex = 1 + NVMIX_FIRST_DATA_BLOCK_INDEX,
    };

    NvmixInode *inodeVirtAddr = (NvmixInode *)((char *)nvmVirtAddr + NVMIX_INODE_BLOCK_OFFSET);

    NvmixInode *rootDirInodeVirtAddr = inodeVirtAddr;
    NvmixInode *fileInodeVirtAddr = inodeVirtAddr + 1;

    *rootDirInodeVirtAddr = rootDirInode;
    *fileInodeVirtAddr = fileInode;

    // 注意：msync() 的参数给定的地址是需要页对齐的。因此这个地方的写入需要合并在一起，不能分开，否则 msync 同步 fileInodeVirtAddr 会失败。
    // 内核的 clflush_cache_range() 的地址不需要页对齐。
    res = msync(inodeVirtAddr, 2 * sizeof(NvmixInode), MS_SYNC);
    if (-1 == res)
    {
        perror("msync");


        return EXIT_FAILURE;
    }

    munmap(nvmVirtAddr, nvmPhySize);

    close(nvmFd);


    // 写入 SSD 上的目录项数据。
    int ssdFd = open(ssdDevicePath, O_RDWR);
    if (-1 == ssdFd)
    {
        perror("open");


        return EXIT_FAILURE;
    }

    // 将本文件系统管理的所有数据块清空。
    const char zeroStr[NVMIX_BLOCK_SIZE] = {0};

    // write() 执行以后文件偏移量会自动往后移，因此不用手动设置。
    // 上个版本这里是 nvmFd，是不对的，应该是 ssdFd，但是程序没有跑出问题。原因是前面释放了 nvmFd 文件描述符对应的文件，但是变量 nvmFd 没有清空，例如是 3。新打开的文件优先使用空闲的文件描述符，因此 ssdFd 的值也是 3，这样歪打正着就成了，但是实际上是不符合语义的。
    for (int i = 0; i < NVMIX_MAX_INODE_NUM; ++i) write(ssdFd, zeroStr, sizeof(zeroStr));

    NvmixDentry fileDentry = {
        .m_ino = 1,
    };
    strcpy(fileDentry.m_name, "reserved.txt");

    lseek(ssdFd, NVMIX_FIRST_DATA_BLOCK_INDEX * NVMIX_BLOCK_SIZE, SEEK_SET);

    write(ssdFd, &fileDentry, sizeof(NvmixDentry));

    close(ssdFd);


    return 0;
}
