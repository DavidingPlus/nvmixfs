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

    void *nvmVirtAddr = mmap(NULL, nvmPhySize, PROT_READ | PROT_WRITE, MAP_SHARED, nvmFd, 0);
    if (MAP_FAILED == nvmVirtAddr)
    {
        perror("mmap");

        close(nvmFd);


        return EXIT_FAILURE;
    }


    NvmixSuperBlock nsb = {
        .m_magic = NVMIX_MAGIC_NUMBER,
        // 直接访问块设备就不会走 vfs 这一层了，所以初始化的时候 m_imap 需要考虑 a.txt，写为 3 而不是 1。
        .m_imap = 0x03,
        .m_version = NvmixVersion{
            .m_major = NVMIX_CONFIG_VERSION_MAJOR,
            .m_minor = NVMIX_CONFIG_VERSION_MINOR,
            .m_alter = NVMIX_CONFIG_VERSION_ALTER,
        },
    };

    NvmixSuperBlock *superBlockVirtAddr = (NvmixSuperBlock *)((char *)nvmVirtAddr + NVMIX_SUPER_BLOCK_OFFSET);

    *superBlockVirtAddr = nsb;
    msync(superBlockVirtAddr, sizeof(NvmixSuperBlock), MS_SYNC);


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
        .m_dataBlockIndex = NVMIX_FIRST_DATA_BLOCK_INDEX + 1,
    };

    NvmixInode *inodeVirtAddr = (NvmixInode *)((char *)nvmVirtAddr + NVMIX_INODE_BLOCK_OFFSET);

    *inodeVirtAddr = rootDirInode;
    msync(inodeVirtAddr, sizeof(NvmixInode), MS_SYNC);

    *(inodeVirtAddr + 1) = fileInode;
    msync(inodeVirtAddr + 1, sizeof(NvmixInode), MS_SYNC);
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
    const char zeroBuf[NVMIX_BLOCK_SIZE] = {0};
    // write() 执行以后文件偏移量会自动往后移，因此不用手动设置。
    for (int i = 0; i < NVMIX_MAX_INODE_NUM; ++i) write(nvmFd, zeroBuf, sizeof(zeroBuf));


    NvmixDentry fileDentry = {
        .m_ino = 1,
    };
    strcpy(fileDentry.m_name, "a.txt");


    lseek(ssdFd, NVMIX_FIRST_DATA_BLOCK_INDEX * NVMIX_BLOCK_SIZE, SEEK_SET);

    write(ssdFd, &fileDentry, sizeof(NvmixDentry));

    close(ssdFd);


    return 0;
}
