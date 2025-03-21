#include <gtest/gtest.h>

#include "util.h"


TEST(nvmixCalcInodeBlocksTest, Test1)
{
    EXPECT_EQ(nvmixCalcInodeBlocks(0), 0);
}

// TEST(nvmixCalcInodeBlocksTest, Test2)
// {
//     EXPECT_EQ(nvmixCalcInodeBlocks(1), 8); // 1字节需要1个块 -> 4096/512=8
// }

// TEST(nvmixCalcInodeBlocksTest, Test3)
// {
//     EXPECT_EQ(nvmixCalcInodeBlocks(4095), 8);
// }

// TEST(nvmixCalcInodeBlocksTest, Test4)
// {
//     EXPECT_EQ(nvmixCalcInodeBlocks(4096), 8);
// }

// TEST(nvmixCalcInodeBlocksTest, Test5)
// {
//     EXPECT_EQ(nvmixCalcInodeBlocks(4097), 16); // 需要2个块 -> 2*8=16
// }

// TEST(nvmixCalcInodeBlocksTest, Test6)
// {
//     EXPECT_EQ(nvmixCalcInodeBlocks(3 * 4096), 24); // 3块 -> 3*8=24
// }

// TEST(nvmixCalcInodeBlocksTest, Test7)
// {
//     EXPECT_EQ(nvmixCalcInodeBlocks(3 * 4096 - 1), 24);
// }

// TEST(nvmixCalcInodeBlocksTest, Test8)
// {
//     EXPECT_EQ(nvmixCalcInodeBlocks(100 * 4096), 800); // 100块 -> 100*8=800
// }
