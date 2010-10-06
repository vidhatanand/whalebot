#ifndef RESPONSE_H
#define RESPONSE_H

#include <vector>
#include <string>
#include <cookie.h>


class CResponse {
public:
    unsigned int        m_iHttpStatusCode;

    //header part
    //Redirect url
    TCookieJar          m_vCookieToSet;
    unsigned int        m_iContentLength;
    std::string         m_sContentType;

    bool                m_isResponseInBody;
    std::vector<char>   m_vBody;
};

#endif // RESPONSE_H
