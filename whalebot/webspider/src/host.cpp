#include <neon/ne_redirect.h>

#include <http_const.h>
#include <host.h>
#include <version.h>

static const CHost::TMoment kNeverBefore    =   boost::posix_time::neg_infin;

static const char           kRoot[]         =   "/";
static const char           kRobotsTxt[]    =   "/robots.txt";

using namespace boost::posix_time;

CHost::CHost( const std::string& authority )
: m_sAuthority(authority)
, m_vPathFetched(0, "")
, m_vPathToFetch(0, "")
, m_pConnection(0)
, m_tTimeOfLastFetch(kNeverBefore)
, m_vActiveCookies(0, CCookie())
{
    //add default paths
    m_vPathFetched.push_back(kRobotsTxt);
    m_vPathToFetch.push_back(kRobotsTxt);
    m_vPathFetched.push_back(kRoot);
    m_vPathToFetch.push_back(kRoot);
}

const CHost& CHost::operator = ( const CHost& another)
{
    if (&another == this) {
        return *this;
    }

    m_sAuthority        =   another.m_sAuthority;
    m_vPathFetched      =   another.m_vPathFetched;
    m_vPathToFetch      =   another.m_vPathToFetch;
    m_pConnection       =   another.m_pConnection;
    m_tTimeOfLastFetch  =   another.m_tTimeOfLastFetch;
    m_vActiveCookies    =   another.m_vActiveCookies;

    return *this;
}

bool CHost::isItTimeToFetch() const
{
    if (kNeverBefore != m_tTimeOfLastFetch) {
        return true;
    }

    TMoment         now(microsec_clock::universal_time());
    time_period     period(m_tTimeOfLastFetch, now);
    unsigned int    timePassedInMcrSec(period.length().total_microseconds());

    return timePassedInMcrSec > kDefaultWaitInMcrSec;
}

void CHost::connect()
{
    //*TODO* move user agent to the options
    static std::string  userAgent(kProductName + " / " + kVersion);

    if (0 != m_pConnection) {
        return;
    }

    m_pConnection   =   ne_session_create( kMainProtocol
                                         , m_sAuthority.c_str()
                                         , kDefaultPort );

    ne_redirect_register(m_pConnection);//register for custom redirect handling
    ne_set_connect_timeout(m_pConnection, kDefaultConnectionTimeoutInSec);
    ne_set_useragent(m_pConnection, userAgent.c_str());
    ne_set_read_timeout(m_pConnection, kDefaultReadTimeoutInSec);
}

void CHost::disconnect()
{
    ne_session_destroy(m_pConnection);
    m_pConnection           =   0;
}
\
bool CHost::addPath( const std::string& path )
{
    bool    pathAdded(false);
    if (std::find(m_vPathFetched.begin(), m_vPathFetched.end(), path) == m_vPathFetched.end()) {
        pathAdded   =   true;
        m_vPathFetched.push_back(path);
        m_vPathToFetch.push_back(path);
    }

    return pathAdded;
}


ne_request* CHost::createRequest()
{
    ne_request* ret(0);

    if (true == m_vPathToFetch.empty()) {
        return ret;
    }

    ret =   ne_request_create( m_pConnection
                             , kMainMethod
                             , m_vPathToFetch.back().c_str() );

    m_vPathToFetch.pop_back();

    for (unsigned int i = 0; i != m_vActiveCookies.size(); ++i) {
        const CCookie&  cookie(m_vActiveCookies[i]);
        ne_add_request_header(ret, kCookieField, cookie.m_sValue.c_str());
    }

    return ret;
}

bool CHost::addCookie( const CCookie& cookie )
{
    bool                ret(false);
    const std::string&  targetDomain(cookie.m_sDomain);
    if (targetDomain.compare( targetDomain.size() > m_sAuthority.size() ? 1 : 0 //because cookie for .google.com ok for
                            , m_sAuthority.size()
                            , m_sAuthority ) == 0) {
        ret =   true;
        m_vActiveCookies.push_back(cookie);
    }

    return ret;
}
