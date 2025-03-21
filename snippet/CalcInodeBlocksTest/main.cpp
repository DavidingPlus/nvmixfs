#include <iostream>

#include "defs.h"
#include "util.h"


int main()
{
    std::cout << nvmixCalcInodeBlocks(0) << std::endl;                    // 0
    std::cout << nvmixCalcInodeBlocks(1) << std::endl;                    // 1
    std::cout << nvmixCalcInodeBlocks(NVMIX_BLOCK_SIZE - 1) << std::endl; // 1
    std::cout << nvmixCalcInodeBlocks(NVMIX_BLOCK_SIZE) << std::endl;     // 1
    std::cout << nvmixCalcInodeBlocks(NVMIX_BLOCK_SIZE + 1) << std::endl; // 2


    return 0;
}
