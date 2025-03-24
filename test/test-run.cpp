#include <gtest/gtest.h>


TEST(TestRunTest, Test1)
{
    SUCCEED();
}

TEST(TestRunTest, Test2)
{
    EXPECT_EQ(sizeof(unsigned char), 1);

    EXPECT_EQ(sizeof(unsigned int), 4);

    EXPECT_EQ(sizeof(unsigned long), 8);
}
