#ifndef WHALEBOT_HOST_TESTS_H
#define WHALEBOT_HOST_TESTS_H

#include <host.h>
#include <gtest/gtest.h>

TEST(HostTest, AddExactCookie)
{
    CHost   host("google.com");
    CCookie exactCookie("", "", "", "google.com");
    EXPECT_TRUE(host.addCookie(exactCookie));
}

TEST(HostTest, AddCommonCookie)
{
    CHost   host("google.com");
    CCookie commonCookie("", "", "", ".google.com");
    EXPECT_TRUE(host.addCookie(commonCookie));
}

TEST(HostTest, AddWrongCookie)
{
    CHost   host("google.com");
    CCookie commonCookie("", "", "", "gaogle.com");
    EXPECT_FALSE(host.addCookie(commonCookie));
}

#endif // WHALEBOT_HOST_TESTS_H
