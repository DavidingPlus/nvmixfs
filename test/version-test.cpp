#include <gtest/gtest.h>
#include <string>

#include "config.h"


TEST(VersionTest, Test1)
{
    std::cout << NVMIX_CONFIG_VERSION << std::endl;

    std::cout << NVMIX_CONFIG_VERSION_MAJOR << std::endl;

    std::cout << NVMIX_CONFIG_VERSION_MINOR << std::endl;

    std::cout << NVMIX_CONFIG_VERSION_ALTER << std::endl;
}

TEST(VersionTest, Test2)
{
    unsigned char major = '0' + NVMIX_CONFIG_VERSION_MAJOR;
    unsigned char minor = '0' + NVMIX_CONFIG_VERSION_MINOR;
    unsigned char alter = '0' + NVMIX_CONFIG_VERSION_ALTER;

    std::string version;

    version += major;
    version += '.';
    version += minor;
    version += '.';
    version += alter;


    EXPECT_EQ(NVMIX_CONFIG_VERSION, version);
}
