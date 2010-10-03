#ifndef WHALEBOT_HOST_H
#define WHALEBOT_HOST_H

#include <neon/ne_session.h>
#include <neon/ne_request.h>

#include <string>
#include <vector>

#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <cookie.h>

typedef std::vector<std::string>    TStringBuffer;

class CHost {
public:

    //TODO add serialization/deserialization methods

    typedef boost::posix_time::ptime    TMoment;

    CHost( const std::string& authority );

    CHost( const CHost& another )
    : m_sAuthority(another.m_sAuthority)
    , m_vPathFetched(another.m_vPathFetched)
    , m_vPathToFetch(another.m_vPathToFetch)
    , m_pConnection(another.m_pConnection)
    , m_tTimeOfLastFetch(another.m_tTimeOfLastFetch)
    , m_vCookieJar(another.m_vCookieJar)
    {}

    bool operator == ( const CHost& another ) const
    {
        return m_sAuthority == another.m_sAuthority;
    }

    bool operator != ( const CHost& another ) const
    {
        return m_sAuthority != another.m_sAuthority;
    }

    bool operator < ( const CHost& another ) const
    {
        return m_sAuthority < another.m_sAuthority;
    }

    bool operator > ( const CHost& another ) const
    {
        return m_sAuthority > another.m_sAuthority;
    }

    const CHost& operator = ( const CHost& another);

    bool isHaveSmthToFetch() const
    {
        return false == m_vPathToFetch.empty();
    }

    bool isItTimeToFetch() const;

    void connect();
    void disconnect();
    bool addPath( const std::string& path );
    ne_request* createRequest();


private:

    typedef std::vector<CCookie>    TActiveCookies;


    std::string     m_sAuthority;
    TStringBuffer   m_vPathFetched;
    TStringBuffer   m_vPathToFetch;
    ne_session*     m_pConnection;
    TMoment         m_tTimeOfLastFetch;
    TActiveCookies  m_vCookieJar;
};

#endif // WHALEBOT_HOST_H
