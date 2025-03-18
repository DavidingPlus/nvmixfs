#include <gtest/gtest.h>

#include "defs.h"


TEST(DefsTest, TestSuperBlock)
{
    // TODO 测试 super_block 区会不会溢出。
}

TEST(DefsTest, TestInode)
{
    EXPECT_EQ(sizeof(struct NvmixInode), 20);
    EXPECT_EQ(sizeof(struct NvmixInode) * NVMIX_MAX_INODE_NUM, 640);
    EXPECT_TRUE(sizeof(struct NvmixInode) * NVMIX_MAX_INODE_NUM < 4096);
}

TEST(DefsTest, TestDataBlock)
{
    EXPECT_EQ(sizeof(struct NvmixDentry), 24);
    EXPECT_EQ(sizeof(struct NvmixDentry) * NVMIX_MAX_INODE_NUM, 768);
    EXPECT_TRUE(sizeof(struct NvmixDentry) * NVMIX_MAX_INODE_NUM < 4096);
}
