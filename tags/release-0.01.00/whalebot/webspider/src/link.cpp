#include <iostream>

#include <boost/algorithm/string.hpp>

#include <parserbase.h>

#include "link.h"




CLink::CLink()
:m_server(""), m_uri(""), m_cookie(""), m_cookieForCut(""){}

server_t  const &CLink::getServer()const{
    return m_server;
}
uri_t  const &CLink::getUri()const{
    return m_uri;
}
std::string  const &CLink::getCookie()const{
    return m_cookie;
}

std::string const &CLink::getCookieForCut()const{
    return m_cookieForCut;
}

void CLink::setServer(server_t const &server){
    m_server    =   server;
}
void CLink::setUri(uri_t const &uri){
    m_uri   =   uri;

}
void CLink::setCookie(std::string const &cookie){
    m_cookie    =   cookie;
    if(!m_cookie.empty()){
        CParserBase   parser(";");
        parser.parseString(m_cookie);
        m_cookieForCut  =   parser.unsafe_getElement(0);
        boost::to_lower(m_cookieForCut);
    }
}

bool CLink::isValid()const{
    return ((!m_server.empty())&&(!m_uri.empty()));
}
void CLink::nil(){
    m_server.clear();
    m_uri.clear();
    m_cookie.clear();
    m_cookieForCut.clear();
}

std::string CLink::toString()const {
    std::string ret("http://");
    ret +=  m_server;
    ret +=  m_uri;
    return ret;
}


std::ostream& operator<<(std::ostream& s,  CLink const & t){
    s<<t.getServer()<<std::endl;
    s<<t.getUri()<<std::endl;
    s<<t.getCookie()<<std::endl;

    return s;
}
std::istream& operator>>(std::istream& s,  CLink & t){
    t.nil();
    std::string buff("");

    if(std::getline(s, buff)){
        t.setServer(buff);
    }

    if(std::getline(s, buff)){
        t.setUri(buff);
    }

    if(std::getline(s, buff)){
        t.setCookie(buff);
    }

    return s;

}
bool CLink::operator==(CLink const& another)const{
    return ((m_server == another.m_server)&&(m_uri == another.m_uri));
}
