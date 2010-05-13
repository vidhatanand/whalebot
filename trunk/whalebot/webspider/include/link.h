/* 
 * File:   link.h
 * Author: hotdox
 *
 * Created on March 21, 2009, 8:45 PM
 */

#ifndef _LINK_H
#define	_LINK_H
#include <string>
#include <boost/functional/hash.hpp>


typedef std::string server_t;
typedef std::string uri_t;



class CLink{
public:
                        CLink();

    server_t const &    getServer()const;
    uri_t const &       getUri()const;
    std::string const & getCookie()const;
    std::string const & getCookieForCut()const;
    std::string         toString()const;

    void                setServer(server_t const &server);
    void                setUri(uri_t const &uri);
    void                setCookie(std::string const &cookie);
    bool                isValid()const;
    void                nil();
    bool                operator==(CLink const& another)const;

private:
    server_t    m_server;
    uri_t       m_uri;
    std::string m_cookie,
                m_cookieForCut;
};

std::ostream& operator<<(std::ostream& s,  CLink const & t);
std::istream& operator>>(std::istream& s,  CLink & t);

struct ihash
    : std::unary_function<CLink, std::size_t>
{
    std::size_t operator()(CLink const& x) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, x.getServer());
        boost::hash_combine(seed, x.getUri());
        return seed;

    }
};

#endif	/* _LINK_H */

