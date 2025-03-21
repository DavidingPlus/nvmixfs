#include <gtest/gtest.h>

#include "util.h"
#include "defs.h"


TEST(UtilTest, NvmixCalcInodeBlocksTest1)
{
    EXPECT_EQ(nvmixCalcInodeBlocks(0), 0);
}

TEST(UtilTest, NvmixCalcInodeBlocksTest2)
{
    EXPECT_EQ(nvmixCalcInodeBlocks(1), 8);
}

TEST(UtilTest, NvmixCalcInodeBlocksTest3)
{
    EXPECT_EQ(nvmixCalcInodeBlocks(NVMIX_BLOCK_SIZE - 1), 8);
}

TEST(UtilTest, NvmixCalcInodeBlocksTest4)
{
    EXPECT_EQ(nvmixCalcInodeBlocks(NVMIX_BLOCK_SIZE), 8);
}

TEST(UtilTest, NvmixCalcInodeBlocksTest5)
{
    EXPECT_EQ(nvmixCalcInodeBlocks(NVMIX_BLOCK_SIZE + 1), 16);
}

TEST(UtilTest, NvmixCalcInodeBlocksTest6)
{
    EXPECT_EQ(nvmixCalcInodeBlocks(3 * NVMIX_BLOCK_SIZE), 24);
}

TEST(UtilTest, NvmixCalcInodeBlocksTest7)
{
    EXPECT_EQ(nvmixCalcInodeBlocks(3 * NVMIX_BLOCK_SIZE - 1), 24);
}

TEST(UtilTest, NvmixCalcInodeBlocksTest8)
{
    EXPECT_EQ(nvmixCalcInodeBlocks(100 * NVMIX_BLOCK_SIZE), 800);
}
