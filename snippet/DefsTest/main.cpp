#include <iostream>

#include "defs.h"


int main()
{
    // 测试 super_block 区会不会溢出。
    std::cout << sizeof(struct NvmixSuperBlock) << std::endl;          // 24
    std::cout << (sizeof(struct NvmixSuperBlock) < 4096) << std::endl; // 1, true

    std::cout << std::endl;

    // 测试 inode 区会不会溢出。
    std::cout << sizeof(struct NvmixInode) << std::endl;                                // 20
    std::cout << sizeof(struct NvmixInode) * NVMIX_MAX_INODE_NUM << std::endl;          // 20 * 32 = 640
    std::cout << (sizeof(struct NvmixInode) * NVMIX_MAX_INODE_NUM < 4096) << std::endl; // 1, true

    std::cout << std::endl;

    // 测试目录的数据块区会不会溢出。
    std::cout << sizeof(struct NvmixDentry) << std::endl;                                // 24
    std::cout << sizeof(struct NvmixDentry) * NVMIX_MAX_INODE_NUM << std::endl;          // 24 * 32 = 768
    std::cout << (sizeof(struct NvmixDentry) * NVMIX_MAX_INODE_NUM < 4096) << std::endl; // 1, true


    return 0;
}
