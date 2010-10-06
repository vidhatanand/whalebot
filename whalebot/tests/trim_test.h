#ifndef BASELIB_STRING_TOKEN_TRIM_TEST
#define BASELIB_STRING_TOKEN_TRIM_TEST

#include <gtest/gtest.h>

#include <trim.h>
#include <standard_generators.h>



static const char   kSpaces[] = "xyz";

static const char   kBoth[] =   "xyzaaaaayyyy";
static const char   kLeft[] =   "xyzaaaaa";

   
TEST(Trim, Both)
{
    CAnsiStrGen                 spacesGen(kSpaces);
    CAnsiTokens::CTokenLimit    beforeTrim(kBoth, kBoth + 12);
    beforeTrim  =   gTrimSequence(beforeTrim, spacesGen);

    EXPECT_STREQ( std::string(beforeTrim.m_Begin, beforeTrim.m_End).c_str()
                , "aaaaa" );
        
}

TEST(Trim, Left)
{
    CAnsiStrGen                 spacesGen(kSpaces);
    CAnsiTokens::CTokenLimit    beforeTrim(kLeft, kLeft + 8);
    beforeTrim  =   gTrimSequence(beforeTrim, spacesGen);

    EXPECT_STREQ( std::string(beforeTrim.m_Begin, beforeTrim.m_End).c_str()
                , "aaaaa" );

}

TEST(Trim, Rigth)
{
    CAnsiStrGen                 spacesGen(kSpaces);
    CAnsiTokens::CTokenLimit    beforeTrim(kLeft, kLeft + 8);
    beforeTrim  =   gTrimSequence(beforeTrim, spacesGen);

    EXPECT_STREQ( std::string(beforeTrim.m_Begin, beforeTrim.m_End).c_str()
                , "aaaaa" );

}

TEST(Trim, Spaces)
{
    CAnsiStrGen                 spacesGen(kSpaces);
    CAnsiTokens::CTokenLimit    beforeTrim(kSpaces, kSpaces + 3);
    beforeTrim  =   gTrimSequence(beforeTrim, spacesGen);

    EXPECT_EQ( beforeTrim.size(), 0 );

}

#endif// BASELIB_STRING_TOKEN_TRIM_TEST
