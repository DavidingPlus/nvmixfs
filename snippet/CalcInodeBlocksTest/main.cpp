#include <iostream>

#include "defs.h"
#include "util.h"


int main()
{
    std::cout << nvmixCalcInodeBlocks(0) << std::endl;                    // 0
    std::cout << nvmixCalcInodeBlocks(1) << std::endl;                    // 8
    std::cout << nvmixCalcInodeBlocks(NVMIX_BLOCK_SIZE - 1) << std::endl; // 8
    std::cout << nvmixCalcInodeBlocks(NVMIX_BLOCK_SIZE) << std::endl;     // 8
    std::cout << nvmixCalcInodeBlocks(NVMIX_BLOCK_SIZE + 1) << std::endl; // 16


    return 0;
}
