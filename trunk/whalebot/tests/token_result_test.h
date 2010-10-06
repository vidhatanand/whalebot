#ifndef BASELIB_STRING_TOKEN_RESULT_TEST
#define BASELIB_STRING_TOKEN_RESULT_TEST

#include <gtest/gtest.h>

#include <token_result.h>

typedef CTokens<int>            CIntTokens;
typedef CIntTokens::CTokenLimit CIntLimits;

TEST(TokenResult, GetPart)
{
    CIntTokens  tokens;
    CIntLimits  limits;
    tokens.addToken(0, 1);
    tokens.addToken(1, 2);
    tokens.addToken(2, 3);

    EXPECT_TRUE(tokens.getPart(0, 2, limits));
    EXPECT_EQ(limits.m_Begin, 0);
    EXPECT_EQ(limits.m_End, 3);

    EXPECT_FALSE(tokens.getPart(5, 2, limits));

    EXPECT_FALSE(tokens.getPart(1, 5, limits));

    EXPECT_FALSE(tokens.getPart(5, 6, limits));
}

TEST(TokenResult, GetHead)
{
    CIntTokens  tokens;
    CIntLimits  limits;
    tokens.addToken(0, 1);
    tokens.addToken(1, 2);
    tokens.addToken(2, 3);

    EXPECT_TRUE(tokens.getHead(1, limits));
    EXPECT_EQ(limits.m_Begin, 0);
    EXPECT_EQ(limits.m_End, 2);

    EXPECT_FALSE(tokens.getHead(5, limits));

}

TEST(TokenResult, GetTail)
{
    CIntTokens  tokens;
    CIntLimits  limits;
    tokens.addToken(0, 1);
    tokens.addToken(1, 2);
    tokens.addToken(2, 3);

    EXPECT_TRUE(tokens.getTail(1, limits));
    EXPECT_EQ(limits.m_Begin, 1);
    EXPECT_EQ(limits.m_End, 3);

    EXPECT_FALSE(tokens.getTail(5, limits));
}


#endif //BASELIB_STRING_TOKEN_RESULT_TEST

