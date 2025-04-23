#include <iostream>
#include <cstring>

#include <sys/stat.h>
#include <linux/types.h>

#include "config.h"

#include "defs.h"


int main(int argc, char const *argv[])
{
    FILE *file = NULL;
    char buffer[NVMIX_BLOCK_SIZE] = {0};
    struct NvmixSuperBlock msb;
    struct NvmixInode rootDirInode;
    struct NvmixInode fileInode;
    struct NvmixDentry fileDentry;
    int i = 0;

    if (2 != argc)
    {
        std::cerr << "Usage: " << argv[0] << " block_device_name\n";

        exit(EXIT_FAILURE);
    }

    file = fopen(argv[1], "w+");
    if (file == NULL)
    {
        perror("fopen");

        exit(EXIT_FAILURE);
    }

    memset(&msb, 0, sizeof(struct NvmixSuperBlock));

    msb.m_magic = NVMIX_MAGIC_NUMBER;

    msb.m_version.m_major = NVMIX_CONFIG_VERSION_MAJOR;
    msb.m_version.m_minor = NVMIX_CONFIG_VERSION_MINOR;
    msb.m_version.m_alter = NVMIX_CONFIG_VERSION_ALTER;

    // 直接访问块设备就不会走 vfs 这一层了，所以初始化的时候 m_imap 需要考虑 a.txt，写为 3 而不是 1。
    msb.m_imap = 0x03;

    // zero disk
    memset(buffer, 0, NVMIX_BLOCK_SIZE);
    for (i = 0; i < 128; i++) fwrite(buffer, 1, NVMIX_BLOCK_SIZE, file);

    fseek(file, 0, SEEK_SET);

    // initialize super block
    fwrite(&msb, sizeof(msb), 1, file);

    // initialize root inode
    memset(&rootDirInode, 0, sizeof(rootDirInode));
    rootDirInode.m_uid = 0;
    rootDirInode.m_gid = 0;
    rootDirInode.m_mode = S_IFDIR | 0755;
    rootDirInode.m_size = 0;
    rootDirInode.m_dataBlockIndex = NVMIX_FIRST_DATA_BLOCK_INDEX;

    fseek(file, NVMIX_INODE_BLOCK_INDEX * NVMIX_BLOCK_SIZE, SEEK_SET);
    fwrite(&rootDirInode, sizeof(rootDirInode), 1, file);

    // initialize new inode
    memset(&fileInode, 0, sizeof(fileInode));
    fileInode.m_uid = 0;
    fileInode.m_gid = 0;
    fileInode.m_mode = S_IFREG | 0644;
    fileInode.m_size = 0;
    fileInode.m_dataBlockIndex = NVMIX_FIRST_DATA_BLOCK_INDEX + 1;
    fwrite(&fileInode, sizeof(fileInode), 1, file);

    // add dentry information
    memset(&fileDentry, 0, sizeof(fileDentry));
    fileDentry.m_ino = 1;
    memcpy(fileDentry.m_name, "a.txt", 5);
    fseek(file, NVMIX_FIRST_DATA_BLOCK_INDEX * NVMIX_BLOCK_SIZE, SEEK_SET);
    fwrite(&fileDentry, sizeof(fileDentry), 1, file);

    fclose(file);


    return 0;
}
