#ifndef BASELIB_STRING_TOKEN_TOKENIZE_STD_TEST
#define BASELIB_STRING_TOKEN_TOKENIZE_STD_TEST

#include <gtest/gtest.h>
#include <tokenize_string.h>
#include "tokenize_const.h"


CStdTokens tokenizeStdString( const std::string& str, const std::string& delim )
{
    CStdStrGen strGen(str.begin(), str.end());
    return tokenizeSequnce(strGen, CStdStrGen(delim.begin(), delim.end()));
}

TEST(TokStd, SimpleList)
{
    CStdTokens tokens(tokenizeStdString("a,b,c", ","));

    EXPECT_EQ(tokens.getLimits().size(), 3);
    EXPECT_EQ(*tokens.getLimits()[0].m_Begin, 'a');
    EXPECT_EQ(*tokens.getLimits()[1].m_Begin, 'b');
    EXPECT_EQ(*tokens.getLimits()[2].m_Begin, 'c');
}

TEST(TokStd, OneChar)
{
    CStdTokens tokens(tokenizeStdString("c", ","));

    EXPECT_EQ(tokens.getLimits().size(), 1);
    EXPECT_EQ(*tokens.getLimits()[0].m_Begin, 'c');
}

TEST(TokStd, SimpleListStartWithDelimiter)
{
    CStdTokens tokens(tokenizeStdString(",a,b,c", ","));

    EXPECT_EQ(tokens.getLimits().size(), 3);
    EXPECT_EQ(*tokens.getLimits()[0].m_Begin, 'a');
    EXPECT_EQ(*tokens.getLimits()[1].m_Begin, 'b');
    EXPECT_EQ(*tokens.getLimits()[2].m_Begin, 'c');
}

TEST(TokStd, SimpleListEndWithDelimiter)
{
    CStdTokens tokens(tokenizeStdString("a,b,c,", ","));

    EXPECT_EQ(tokens.getLimits().size(), 3);
    EXPECT_EQ(*tokens.getLimits()[0].m_Begin, 'a');
    EXPECT_EQ(*tokens.getLimits()[1].m_Begin, 'b');
    EXPECT_EQ(*tokens.getLimits()[2].m_Begin, 'c');
}

TEST(TokStd, ManyDelimiter)
{
    CStdTokens tokens(tokenizeStdString(",,,,,a,,,b,,,,,,c,,,,,,,", ","));

    EXPECT_EQ(tokens.getLimits().size(), 3);
    EXPECT_EQ(*tokens.getLimits()[0].m_Begin, 'a');
    EXPECT_EQ(*tokens.getLimits()[1].m_Begin, 'b');
    EXPECT_EQ(*tokens.getLimits()[2].m_Begin, 'c');
}

TEST(TokStd, ManyCharSimpleList)
{
    CStdTokens tokens(tokenizeStdString("aaa,bb,cccc", ","));

    EXPECT_EQ(tokens.getLimits().size(), 3);

    EXPECT_STREQ( std::string(tokens.getLimits()[0].m_Begin, tokens.getLimits()[0].m_End).c_str()
                , "aaa" );

    EXPECT_STREQ( std::string(tokens.getLimits()[1].m_Begin, tokens.getLimits()[1].m_End).c_str()
                , "bb" );

    EXPECT_STREQ( std::string(tokens.getLimits()[2].m_Begin, tokens.getLimits()[2].m_End).c_str()
                , "cccc" );
}

TEST(TokStd, ManyCharManyDelims)
{
    CStdTokens tokens(tokenizeStdString(",,,,,,,,,,aaa,,bb,,,,,cccc,,,,,,dddddd", ","));

    EXPECT_EQ(tokens.getLimits().size(), 4);

    EXPECT_STREQ( std::string(tokens.getLimits()[0].m_Begin, tokens.getLimits()[0].m_End).c_str()
                , "aaa" );

    EXPECT_STREQ( std::string(tokens.getLimits()[1].m_Begin, tokens.getLimits()[1].m_End).c_str()
                , "bb" );

    EXPECT_STREQ( std::string(tokens.getLimits()[2].m_Begin, tokens.getLimits()[2].m_End).c_str()
                , "cccc" );

    EXPECT_STREQ( std::string(tokens.getLimits()[3].m_Begin, tokens.getLimits()[3].m_End).c_str()
                , "dddddd" );

}

TEST(TokStd, ManyCharManyDelimsVary)
{
    CStdTokens tokens(tokenizeStdString(",,,!!!;;;,aaa,,bb,;;;?,,cccc,,!!@,dddddd", ",;!?@"));

    EXPECT_EQ(tokens.getLimits().size(), 4);

    EXPECT_STREQ( std::string(tokens.getLimits()[0].m_Begin, tokens.getLimits()[0].m_End).c_str()
                , "aaa" );

    EXPECT_STREQ( std::string(tokens.getLimits()[1].m_Begin, tokens.getLimits()[1].m_End).c_str()
                , "bb" );

    EXPECT_STREQ( std::string(tokens.getLimits()[2].m_Begin, tokens.getLimits()[2].m_End).c_str()
                , "cccc" );

    EXPECT_STREQ( std::string(tokens.getLimits()[3].m_Begin, tokens.getLimits()[3].m_End).c_str()
                , "dddddd" );
}

#endif //BASELIB_STRING_TOKEN_TOKENIZE_STD_TEST
