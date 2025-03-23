#include <gtest/gtest.h>

#include "defs.h"


TEST(DefsTest, SuperBlockTest)
{
    EXPECT_EQ(sizeof(struct NvmixSuperBlock), 24);
    EXPECT_TRUE(sizeof(struct NvmixSuperBlock) < 4096);
}

TEST(DefsTest, InodeTest)
{
    EXPECT_EQ(sizeof(struct NvmixInode), 20);
    EXPECT_EQ(sizeof(struct NvmixInode) * NVMIX_MAX_INODE_NUM, 640);
    EXPECT_TRUE(sizeof(struct NvmixInode) * NVMIX_MAX_INODE_NUM < 4096);
}

TEST(DefsTest, DataBlockTest)
{
    EXPECT_EQ(sizeof(struct NvmixDentry), 24);
    EXPECT_EQ(sizeof(struct NvmixDentry) * NVMIX_MAX_INODE_NUM, 768);
    EXPECT_TRUE(sizeof(struct NvmixDentry) * NVMIX_MAX_INODE_NUM < 4096);
}
