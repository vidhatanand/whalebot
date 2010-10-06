#ifndef BASELIB_STRING_TOKEN_GENERATORS_TEST
#define BASELIB_STRING_TOKEN_GENERATORS_TEST

#include <gtest/gtest.h>

#include <standard_generators.h>

TEST(Generator, StdStrFound)
{
    std::string tmp("abcdef");
    CStdStrGen  gen(tmp.begin(), tmp.end());
    EXPECT_TRUE(gFindInGenerator(gen, 'a'));
    EXPECT_TRUE(gFindInGenerator(gen, 'd'));
    EXPECT_TRUE(gFindInGenerator(gen, 'f'));
    EXPECT_FALSE(gFindInGenerator(gen, 'r'));
    EXPECT_FALSE(gFindInGenerator(gen, 'y'));
}

TEST(Generator, AnsiStrFound)
{
    CAnsiStrGen  gen("abcdef");
    EXPECT_TRUE(gFindInGenerator(gen, 'a'));
    EXPECT_TRUE(gFindInGenerator(gen, 'd'));
    EXPECT_TRUE(gFindInGenerator(gen, 'f'));
    EXPECT_FALSE(gFindInGenerator(gen, 'r'));
    EXPECT_FALSE(gFindInGenerator(gen, 'y'));
}



#endif //BASELIB_STRING_TOKEN_GENERATORS_TEST

