#include <set>
#include <iostream>

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string.hpp>

#include <parserbase.h>

#include <link_factory.h>
#include <link.h>
#include <prefix.h>
#include <ilink_buffer.h>


CLinkFactory::CLinkFactory()
:m_parentLink(){}

void CLinkFactory::setAcceptor(IAcceptor &acceptor){
    m_pAcceptor  =   &acceptor;
}

void CLinkFactory::pushLink(std::string link) {

    if((prefix::isMail(link))||(prefix::isJavaScript(link))||(prefix::isHttps(link)))
        return;

    fullCut(link);

    normolizeQeury(link);

    if(link.empty())
        return;

    m_pAcceptor->pushLink(normolizeLink(link));
}

void CLinkFactory::setFrom(CLink const &uri){
    m_parentLink   =   uri;
}


void CLinkFactory::cutSticks(std::string &link){
    std::string ret("");
    ret.reserve(link.size());
    std::string::const_iterator i(link.begin());
    while(i != link.end()){
        while((i != link.end())&&(*i != '/')){
            ret +=  *i;
            ++i;
        }
        if(i != link.end()){

            while((i != link.end())&&(*i == '/')){
                ++i;
            }
        
            ret +=  '/';
        }
    }
    link    =   ret;
}
void CLinkFactory::cutCutId(std::string &link){
    size_t  pos(link.rfind('#'));
        if(pos != std::string::npos)
            link.erase(pos);
}
void CLinkFactory::cutHtml(std::string &link){
//    boost::replace_all(link,"&amp;","&");
    boost::erase_all(link,"\r");
    boost::erase_all(link,"\n");
}

void CLinkFactory::cutCookie(std::string &link)const{

    std::string const &cookie(m_parentLink.getCookieForCut());

    if(cookie.empty())
        return;

    size_t	pos(link.find(cookie));
    if(pos != std::string::npos){
        link.erase(pos, cookie.size());
    }
}
CLink CLinkFactory::normolizeLink(std::string &link)const
{
    CLink    ret;

    if (prefix::isHttp(link)) {
        //full http
        link.erase(0, prefix::http.size());
        size_t pos(link.find('/'));

        if (pos != std::string::npos) {
            ret.setServer(link.substr(0, pos));
            ret.setUri(link.substr(pos));
        } else {
            ret.setServer(link);
            ret.setUri("/");
        }

    } else {
        //relative link
        ret.setServer(m_parentLink.getServer());

        if (link[0] == '/') {
            ret.setUri(link);
        } else {
            std::string uri(m_parentLink.getUri());
            uri +=  '/';
            uri +=  link;
            ret.setUri(uri);
        }
    }
    std::string tmp_uri(ret.getUri());
    //cutSticks(tmp_uri);
    ret.setUri(tmp_uri);

    if(ret.getServer() == m_parentLink.getServer()){
        ret.setCookie(m_parentLink.getCookie());
    }

    return ret;
    
}
void CLinkFactory::fullCut(std::string &link)const
{
    cutHtml(link);
    cutCookie(link);
    cutCutId(link);
}


void CLinkFactory::normolizeQeury(std::string &link)
{
    size_t startQuery(link.rfind('?'));

    if (std::string::npos == startQuery) {
        return;
    }

    std::string query(link.substr(startQuery + 1));
    link.erase(startQuery + 1);    

    CParserBase parser("&=");
    parser.parseString(query);

    size_t  elementCount(parser.size());
    
    
    typedef std::set<std::string>  TQueryParams;
    TQueryParams    params;
    
    for (unsigned int i = 0; i + 1 < elementCount; i += 2) {
        std::string key(parser.unsafe_getElement(i));
        std::string value(parser.unsafe_getElement(i + 1));
        TQueryParams::iterator  curKeyPlace(params.lower_bound(key));
        if ((params.end() == curKeyPlace) or (*curKeyPlace != key)) {
            link    +=  key;
            link    +=  "=";
            link    +=  value;
            link    +=  "&";
            params.insert(curKeyPlace, key);
        }


    }


    if ('&' == link[link.size()-1]) {
        link.erase(link.size() - 1);
    }
}
