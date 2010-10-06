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
    {}

    CCookie( const std::string& value
           , const std::string& appliedPath
           , const std::string& validDate )
    : m_sValue(value)
    , m_sAppliedPath(appliedPath)
    , m_sValidDate(validDate)
    {}

    void clear();
    void fromString( const char* cookieStr );


    std::string m_sValue;
    std::string m_sAppliedPath;
    std::string m_sValidDate;
};

typedef std::vector<CCookie>    TCookieJar;

#endif // WHALEBOT_COOKIE_H
