
#include <neon/ne_redirect.h>

#include <boost/thread.hpp>



#include <header_parser.h>
#include <one_fetcher.h>
#include <whale_link.h>
#include <version.h>
#include <http_const.h>

using namespace boost::posix_time;

const COneFetcher::TMoment COneFetcher::kNeverBefore   =   boost::posix_time::neg_infin;


COneFetcher::COneFetcher()
: m_pConnection(0)
, m_sLastConnectedHost("")
, m_pRequest(0)
, m_tTimeOfLastRequest(kNeverBefore)
{}

bool COneFetcher::connect( CLink const &link )
{
    static std::string  userAgent(kProductName + " / " + kVersion);

    const std::string& targetServer(link.getServer());

    if ((0 != m_pConnection) and (targetServer != m_sLastConnectedHost)) {
        //we need to disconnect from wrong host
        ne_session_destroy(m_pConnection);
        m_pConnection           =   0;
        m_sLastConnectedHost.clear();
    }

    if (0 == m_pConnection) {
        //we need to connect to rigth host

        m_pConnection   =   ne_session_create( kMainProtocol
                                             , targetServer.c_str()
                                             , kDefaultPort );

        ne_redirect_register(m_pConnection);//register for custom redirect handling
        ne_set_connect_timeout(m_pConnection, kDefaultConnectionTimeoutInSec);
        ne_set_useragent(m_pConnection, userAgent.c_str());
        ne_set_read_timeout(m_pConnection, kDefaultReadTimeoutInSec);

        m_sLastConnectedHost    =   targetServer;
        m_tTimeOfLastRequest    =   kNeverBefore;
    }

    return (0 != m_pConnection);
}


bool COneFetcher::request( CLink const &link )
{
    m_pRequest  =   ne_request_create( m_pConnection
                                     , kMainMethod
                                     , link.getUri().c_str() );

    if (not link.getCookieForCut().empty()) {
        ne_add_request_header(m_pRequest, kCookieField, link.getCookieForCut().c_str());
    }

    //limit fetch speed for one site
    if (m_tTimeOfLastRequest != kNeverBefore) {
        TMoment         now(microsec_clock::universal_time());
        unsigned int    timePassedInMcrS(time_period( m_tTimeOfLastRequest, now).length().total_microseconds());
        if (timePassedInMcrS < kDefaultWaitInMcrSec) {
            boost::this_thread::sleep(microseconds(kDefaultWaitInMcrSec - timePassedInMcrS));
        }
    }

    m_tTimeOfLastRequest    =   microsec_clock::universal_time();

    int requestResult(ne_begin_request(m_pRequest));

    return ((NE_OK == requestResult) or (NE_REDIRECT == requestResult));
}

unsigned int COneFetcher::getHeader( CHeaderParser &header, std::ostream &out )
{
    header.setRequest(m_pRequest);
    return ne_get_status(m_pRequest)->code;
}

bool COneFetcher::getResponse( std::ostream &out, unsigned int* bytesFetched )
{

    static char     kReadBuffer[kDefaultReadBufferSizeInBytes];
    unsigned int    bytesFetchedInternal(0);

    ssize_t  readSize;

    while ((readSize = ne_read_response_block(m_pRequest, kReadBuffer, kDefaultReadBufferSizeInBytes)) > 0) {
        out.write(kReadBuffer, readSize);
        bytesFetchedInternal    +=  readSize;
    }

    if (readSize == 0) {
        ne_end_request(m_pRequest);
    }

    ne_request_destroy(m_pRequest);
    m_pRequest  =   0;

    if (0 != bytesFetched) {
        *bytesFetched   =   bytesFetchedInternal;
    }

    return (0 == readSize);
}

COneFetcher::~COneFetcher()
{
    if (0 != m_pRequest) {
        ne_request_destroy(m_pRequest);
        m_pRequest  =   0;
    }

    if (0 != m_pConnection) {
        ne_session_destroy(m_pConnection);
    }
}
