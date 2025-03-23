#include <iostream>
#include <string>

#include "defs.h"


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


int main()
{
    std::string s = hexAsciiToString(0x68656C6C6F);
    std::cout << s << std::endl;              // hello
    std::cout << (s == "hello") << std::endl; // hello

    s = hexAsciiToString(NVMIX_MAGIC_NUMBER);
    std::cout << s << std::endl;              // nvmix
    std::cout << (s == "nvmix") << std::endl; // nvmix


    return 0;
}
