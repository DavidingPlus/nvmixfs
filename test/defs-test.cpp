#include <gtest/gtest.h>

#include "defs.h"


TEST(DefsTest, SuperBlockTest)
{
    // TODO 测试 super_block 区会不会溢出。
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

std::string hexAsciiToString(uint64_t val)
{
    std::string res;

    bool leadingZero = true; // 用于跳过高位的 0

    for (int i = (sizeof(val) - 1) * 8; i >= 0; i -= 8)
    {
        char c = (val >> i) & 0xFF;

        if (c == '\0' && leadingZero) continue; // 跳过前导零

        leadingZero = false;

        if (c == '\0') break; // 遇到真正的字符串结束符 '\0' 终止

        res += c;
    }


    return res;
}

TEST(DefsTest, MagicNumberTest)
{
    // 测试 nvmixfs 魔数。
    EXPECT_EQ(hexAsciiToString(NVMIX_MAGIC_NUMBER), "nvmix"); // "Hello"

    // 测试正常字符串（带隐式终止符）
    EXPECT_EQ(hexAsciiToString(0x48656C6C6F000000), "Hello"); // "Hello"

    // 测试前导零跳过
    EXPECT_EQ(hexAsciiToString(0x000058595A000000), "XYZ"); // "XYZ"

    // 测试全零输入
    EXPECT_EQ(hexAsciiToString(0x0000000000000000), "");

    // 测试中间终止符
    EXPECT_EQ(hexAsciiToString(0x4142004344454647), "AB"); // "AB"（0x43='C'被截断）

    // 测试最长可能字符串（8字符）
    EXPECT_EQ(hexAsciiToString(0x4142434445464748), "ABCDEFGH");

    // 测试尾部非零
    EXPECT_EQ(hexAsciiToString(0x0000000000000041), "A"); // 最低字节为'A'

    // 测试前导零后接非零不含终止符
    EXPECT_EQ(hexAsciiToString(0x0041424344454647), "ABCDEFG"); // 跳过第一个00，读取后续7字节

    // 测试混合零值
    EXPECT_EQ(hexAsciiToString(0x480065004C004C00), "H"); // 只有第一个非零字符'H'被保留
}
