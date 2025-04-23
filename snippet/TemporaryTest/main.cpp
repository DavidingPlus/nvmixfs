#include <iostream>
#include <string>


int main()
{
    std::string s = "0x10000000";

    std::cout << std::stoi(s, nullptr, 16) << std::endl; // 268435456 B，即 256 MB。


    return 0;
}
