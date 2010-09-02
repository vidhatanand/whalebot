#ifndef _LINK_EXTRACTOR_H_
#define _LINK_EXTRACTOR_H_

#include <iterator>
#include <iostream>
#include <string>

#include <htmlcxx/html/ParserSax.h>
#include <htmlcxx/html/utils.h>

#include <prefix.h>
#include <link.h>
#include <http_const.h>

struct TLinksPlace {
    const char* m_pTag;
    const char* m_pAttribute;
};

template <class T>
class CLinkExtractor : public htmlcxx::HTML::ParserSax {
public:

    CLinkExtractor(T &out)
    :m_tLinkAcceptor(out)
    {}

    void extract(std::istream& in)
    {

        static char     tmpBuffer[kDefaultReadBufferSizeInBytes];
        std::string     buff("");
        unsigned int    readSize(0);

        while ((readSize = in.readsome(tmpBuffer, kDefaultReadBufferSizeInBytes)) > 0) {
            buff.append(tmpBuffer, readSize);
        }

        parse(buff);
    }

    static bool isParse(const std::string &ext)
    {
        return ext == "html";
    }

protected:

    void foundTag(htmlcxx::HTML::Node node, bool isEnd)
    {

        static TLinksPlace places[] =  { {"a", "href"}
                                       , {"frame", "src"}
                                       , {"img", "src"}
                                       , {"area", "href"}
                                       , {"link", "href"} };

        static unsigned int placesCount(5);
        
        unsigned int    i(0);

        while ((i < placesCount) and (node.tagName() != places[i].m_pTag)) {
            ++i;
        }

        if (i < placesCount) {//stop on second condition
        
            node.parseAttributes();
            std::pair<bool, std::string > res(node.attribute(places[i].m_pAttribute));
            if (res.first) {
                m_tLinkAcceptor.pushLink(htmlcxx::HTML::decode_entities(res.second));
            }
        }        
    }
    
private:
    T&  m_tLinkAcceptor;
};

#endif //_LINK_EXTRACTOR_H_
