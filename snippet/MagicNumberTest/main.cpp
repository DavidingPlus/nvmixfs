#include <iostream>
#include <string>

#include "defs.h"


std::string hexToString(uint64_t hexValue)
{
    std::string res;

    bool leadingZero = true; // 用于跳过高位的 0

    for (int i = (sizeof(hexValue) - 1) * 8; i >= 0; i -= 8)
    {
        char c = (hexValue >> i) & 0xFF;

        if (c == '\0' && leadingZero) continue; // 跳过前导零

        leadingZero = false;

        if (c == '\0') break; // 遇到真正的字符串结束符 '\0' 终止

        res += c;
    }


    return res;
}


int main()
{
    std::string s = hexToString(0x68656C6C6F);
    std::cout << s << std::endl;              // hello
    std::cout << (s == "hello") << std::endl; // hello

    s = hexToString(NVMIX_MAGIC_NUMBER);
    std::cout << s << std::endl;              // nvmix
    std::cout << (s == "nvmix") << std::endl; // nvmix


    return 0;
}
