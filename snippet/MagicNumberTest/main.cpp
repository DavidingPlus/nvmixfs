#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include "defs.h"


std::string asciiBytesToString(uint64_t val)
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


TEST(MagicNumberTest, Test1)
{
    // 测试 nvmixfs 的魔数。
    EXPECT_EQ(asciiBytesToString(NVMIX_MAGIC_NUMBER), "nvmix"); // "nvmix"

    // 测试正常字符串。
    EXPECT_EQ(asciiBytesToString(0x48656C6C6F), "Hello"); // "Hello"

    // 测试正常字符串（带隐式终止符）。
    EXPECT_EQ(asciiBytesToString(0x48656C6C6F000000), "Hello"); // "Hello"

    // 测试前导零跳过。
    EXPECT_EQ(asciiBytesToString(0x000058595A000000), "XYZ"); // "XYZ"

    // 测试全零输入。
    EXPECT_EQ(asciiBytesToString(0x0000000000000000), "");

    // 测试中间终止符。
    EXPECT_EQ(asciiBytesToString(0x4142004344454647), "AB"); // "AB"（0x43 == 'C' 被截断）。

    // 测试最长可能字符串（8 字符）。
    EXPECT_EQ(asciiBytesToString(0x4142434445464748), "ABCDEFGH");

    // 测试尾部非零。
    EXPECT_EQ(asciiBytesToString(0x0000000000000041), "A"); // 最低字节为 'A'。

    // 测试前导零后接非零不含终止符。
    EXPECT_EQ(asciiBytesToString(0x0041424344454647), "ABCDEFG"); // 跳过第一个 00，读取后续 7 字节。

    // 测试混合零值。
    EXPECT_EQ(asciiBytesToString(0x480065004C004C00), "H"); // 只有第一个非零字符 'H' 被保留。
}
