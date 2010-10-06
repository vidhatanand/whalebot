#ifndef BASELIB_STRING_TOKEN_TOKENIZE_ANSI_TEST
#define BASELIB_STRING_TOKEN_TOKENIZE_ANSI_TEST

#include <gtest/gtest.h>
#include <tokenize_string.h>

#include "tokenize_const.h"

CAnsiTokens tokenizeAnsiString( const char* str, const char* delim )
{
    CAnsiStrGen strGen(str);
    return tokenizeSequnce(strGen, CAnsiStrGen(delim));
}

TEST(TokAnsi, SimpleList)
{
    CAnsiTokens tokens(tokenizeAnsiString("a,b,c", ","));

    EXPECT_EQ(tokens.getLimits().size(), 3);
    EXPECT_EQ(*tokens.getLimits()[0].m_Begin, 'a');
    EXPECT_EQ(*tokens.getLimits()[1].m_Begin, 'b');
    EXPECT_EQ(*tokens.getLimits()[2].m_Begin, 'c');
}

TEST(TokAnsi, OneChar)
{
    CAnsiTokens tokens(tokenizeAnsiString("c", ","));

    EXPECT_EQ(tokens.getLimits().size(), 1);
    EXPECT_EQ(*tokens.getLimits()[0].m_Begin, 'c');
}

TEST(TokAnsi, SimpleListStartWithDelimiter)
{
    CAnsiTokens tokens(tokenizeAnsiString(",a,b,c", ","));

    EXPECT_EQ(tokens.getLimits().size(), 3);
    EXPECT_EQ(*tokens.getLimits()[0].m_Begin, 'a');
    EXPECT_EQ(*tokens.getLimits()[1].m_Begin, 'b');
    EXPECT_EQ(*tokens.getLimits()[2].m_Begin, 'c');
}

TEST(TokAnsi, SimpleListEndWithDelimiter)
{
    CAnsiTokens tokens(tokenizeAnsiString("a,b,c,", ","));

    EXPECT_EQ(tokens.getLimits().size(), 3);
    EXPECT_EQ(*tokens.getLimits()[0].m_Begin, 'a');
    EXPECT_EQ(*tokens.getLimits()[1].m_Begin, 'b');
    EXPECT_EQ(*tokens.getLimits()[2].m_Begin, 'c');
}

TEST(TokAnsi, ManyDelimiter)
{
    CAnsiTokens tokens(tokenizeAnsiString(",,,,,a,,,b,,,,,,c,,,,,,,", ","));

    EXPECT_EQ(tokens.getLimits().size(), 3);
    EXPECT_EQ(*tokens.getLimits()[0].m_Begin, 'a');
    EXPECT_EQ(*tokens.getLimits()[1].m_Begin, 'b');
    EXPECT_EQ(*tokens.getLimits()[2].m_Begin, 'c');
}

TEST(TokAnsi, ManyCharSimpleList)
{
    CAnsiTokens tokens(tokenizeAnsiString("aaa,bb,cccc", ","));

    EXPECT_EQ(tokens.getLimits().size(), 3);

    EXPECT_TRUE(strncmp( tokens.getLimits()[0].m_Begin
                       , "aaa"
                       , tokens.getLimits()[0].m_End - tokens.getLimits()[0].m_Begin ) == 0);

    EXPECT_TRUE(strncmp( tokens.getLimits()[1].m_Begin
                       , "bb"
                       , tokens.getLimits()[1].m_End - tokens.getLimits()[1].m_Begin ) == 0);

    EXPECT_TRUE(strncmp( tokens.getLimits()[2].m_Begin
                       , "cccc"
                       , tokens.getLimits()[2].m_End - tokens.getLimits()[2].m_Begin ) == 0);
}

TEST(TokAnsi, ManyCharManyDelims)
{
    CAnsiTokens tokens(tokenizeAnsiString(",,,,,,,,,,aaa,,bb,,,,,cccc,,,,,,dddddd", ","));

    EXPECT_EQ(tokens.getLimits().size(), 4);

    EXPECT_TRUE(strncmp( tokens.getLimits()[0].m_Begin
                       , "aaa"
                       , tokens.getLimits()[0].m_End - tokens.getLimits()[0].m_Begin ) == 0);

    EXPECT_TRUE(strncmp( tokens.getLimits()[1].m_Begin
                       , "bb"
                       , tokens.getLimits()[1].m_End - tokens.getLimits()[1].m_Begin ) == 0);

    EXPECT_TRUE(strncmp( tokens.getLimits()[2].m_Begin
                       , "cccc"
                       , tokens.getLimits()[2].m_End - tokens.getLimits()[2].m_Begin ) == 0);

    EXPECT_TRUE(strncmp( tokens.getLimits()[3].m_Begin
                       , "dddddd"
                       , tokens.getLimits()[3].m_End - tokens.getLimits()[3].m_Begin ) == 0);
}

TEST(TokAnsi, ManyCharManyDelimsVary)
{
    CAnsiTokens tokens(tokenizeAnsiString(",,,!!!;;;,aaa,,bb,;;;?,,cccc,,!!@,dddddd", ",;!?@"));

    EXPECT_EQ(tokens.getLimits().size(), 4);

    EXPECT_TRUE(strncmp( tokens.getLimits()[0].m_Begin
                       , "aaa"
                       , tokens.getLimits()[0].m_End - tokens.getLimits()[0].m_Begin ) == 0);

    EXPECT_TRUE(strncmp( tokens.getLimits()[1].m_Begin
                       , "bb"
                       , tokens.getLimits()[1].m_End - tokens.getLimits()[1].m_Begin ) == 0);

    EXPECT_TRUE(strncmp( tokens.getLimits()[2].m_Begin
                       , "cccc"
                       , tokens.getLimits()[2].m_End - tokens.getLimits()[2].m_Begin ) == 0);

    EXPECT_TRUE(strncmp( tokens.getLimits()[3].m_Begin
                       , "dddddd"
                       , tokens.getLimits()[3].m_End - tokens.getLimits()[3].m_Begin ) == 0);
}

#endif //BASELIB_STRING_TOKEN_TOKENIZE_ANSI_TEST
