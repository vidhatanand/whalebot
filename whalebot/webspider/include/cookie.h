#ifndef WHALEBOT_COOKIE_H
#define WHALEBOT_COOKIE_H

#include <string>
#include <vector>

class CCookie {
public:

    CCookie()
    : m_sValue("")
    , m_sAppliedPath("")
    , m_sValidDate("")
    , m_sDomain("")
    {}

    CCookie( const CCookie& another )
    : m_sValue(another.m_sValue)
    , m_sAppliedPath(another.m_sAppliedPath)
    , m_sValidDate(another.m_sValidDate)
    , m_sDomain(another.m_sDomain)
    {}

    CCookie( const char* cookieStr );

    CCookie( const std::string& value
           , const std::string& appliedPath
           , const std::string& validDate
           , const std::string& domain )
    : m_sValue(value)
    , m_sAppliedPath(appliedPath)
    , m_sValidDate(validDate)
    , m_sDomain(domain)
    {}

    void clear();
    void fromString( const char* cookieStr );


    std::string m_sValue;
    std::string m_sAppliedPath;
    std::string m_sValidDate;
    std::string m_sDomain;
};

typedef std::vector<CCookie>    TCookieJar;

#endif // WHALEBOT_COOKIE_H
