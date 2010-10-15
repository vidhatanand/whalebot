#ifndef WHALEBOT_COOKIE_PASRE_TEST_H
#define WHALEBOT_COOKIE_PASRE_TEST_H

#include <cookie.h>
#include <gtest/gtest.h>

TEST(CookieParse, GoogleCookie_1)
{
    CCookie cookie;

    cookie.fromString("PREF=ID=12195d8e497e9ed0:TM=1286311566:LM=1286311566:S=jhzeU3-vq4H4pJs9; expires=Thu, 04-Oct-2012 20:46:06 GMT; path=/; domain=.google.com");
    EXPECT_STREQ( cookie.m_sValue.c_str()
                , "PREF=ID=12195d8e497e9ed0:TM=1286311566:LM=1286311566:S=jhzeU3-vq4H4pJs9");

    EXPECT_STREQ( cookie.m_sValidDate.c_str()
                , "Thu, 04-Oct-2012 20:46:06 GMT");

    EXPECT_STREQ( cookie.m_sAppliedPath.c_str()
                , "/");

    EXPECT_STREQ( cookie.m_sDomain.c_str()
                , ".google.com" );

}

TEST(CookieParse, GoogleCookie_2)
{
    CCookie cookie;

    cookie.fromString("NID=39=Tnkn-UEKzp4OeTMbpcwm_hS61U0k5ug2SA3d8zuXf1GCdSQuMWQWLpJXbsdhHdUMK6i7BxfFsXvrmEv8Se6vVGEYHH40S77ryJQeODemhMWuVcp-SltydMSjugRWwxdW; expires=Wed, 06-Apr-2011 20:46:06 GMT; path=/; domain=.google.com; HttpOnly");
    EXPECT_STREQ( cookie.m_sValue.c_str()
                , "NID=39=Tnkn-UEKzp4OeTMbpcwm_hS61U0k5ug2SA3d8zuXf1GCdSQuMWQWLpJXbsdhHdUMK6i7BxfFsXvrmEv8Se6vVGEYHH40S77ryJQeODemhMWuVcp-SltydMSjugRWwxdW");

    EXPECT_STREQ( cookie.m_sValidDate.c_str()
                , "Wed, 06-Apr-2011 20:46:06 GMT");

    EXPECT_STREQ( cookie.m_sAppliedPath.c_str()
                , "/");

    EXPECT_STREQ( cookie.m_sDomain.c_str()
                , ".google.com" );

}

#endif // WHALEBOT_COOKIE_PASRE_TEST_H
