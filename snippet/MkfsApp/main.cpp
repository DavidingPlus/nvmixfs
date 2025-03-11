#include <iostream>
#include <cstring>

#include <sys/stat.h>
#include <linux/types.h>

#include "defs.h"


int main(int argc, char const *argv[])
{
    FILE *file;
    char buffer[NVMIX_BLOCK_SIZE];
    struct NvmixSuperBlock msb;
    struct NvmixInode rootInode;
    struct NvmixInode fileInode;
    struct NvmixDirEntry fileDentry;
    int i;

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

    msb.magic = NVMIX_MAGIC;
    msb.version = 1;
    msb.imap = 0x03;

    /* zero disk  */
    memset(buffer, 0, NVMIX_BLOCK_SIZE);
    for (i = 0; i < 128; i++) fwrite(buffer, 1, NVMIX_BLOCK_SIZE, file);

    fseek(file, 0, SEEK_SET);

    /* initialize super block */
    fwrite(&msb, sizeof(msb), 1, file);

    /* initialize root inode */
    memset(&rootInode, 0, sizeof(rootInode));
    rootInode.uid = 0;
    rootInode.gid = 0;
    rootInode.mode = S_IFDIR | 0755;
    rootInode.size = 0;
    rootInode.data_block = NVMIX_FIRST_DATA_BLOCK;

    fseek(file, NVMIX_INODE_BLOCK * NVMIX_BLOCK_SIZE, SEEK_SET);
    fwrite(&rootInode, sizeof(rootInode), 1, file);

    /* initialize new inode */
    memset(&fileInode, 0, sizeof(fileInode));
    fileInode.uid = 0;
    fileInode.gid = 0;
    fileInode.mode = S_IFREG | 0644;
    fileInode.size = 0;
    fileInode.data_block = NVMIX_FIRST_DATA_BLOCK + 1;
    fwrite(&fileInode, sizeof(fileInode), 1, file);

    /* add dentry information */
    memset(&fileDentry, 0, sizeof(fileDentry));
    fileDentry.ino = 1;
    memcpy(fileDentry.name, "a.txt", 5);
    fseek(file, NVMIX_FIRST_DATA_BLOCK * NVMIX_BLOCK_SIZE, SEEK_SET);
    fwrite(&fileDentry, sizeof(fileDentry), 1, file);

    fclose(file);


    return 0;
}
