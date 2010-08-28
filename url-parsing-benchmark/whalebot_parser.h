#pragma once

#include <string>

#include "../whalebot/webspider/include/link_factory.h"

#include "answer.h"


namespace whalebot {

class CEmptyAcceptor : public IAcceptor {
public:
    void    pushLink( const CLink& link );
    CLink   m_tLink;
};

void gParseBase( CLinkFactory& factory,
                  CEmptyAcceptor& acceptor,
                  const std::string& baseUrl );

TUrlParseResult gParseRel( CLinkFactory& factory,
                           CEmptyAcceptor& acceptor,
                           const std::string& relativeUrl );


class CParser {
public:

    CParser();
    void parseBase( const std::string& baseUri );
    void parseRel( const std::string& relUri );

private:
    CEmptyAcceptor  m_tAcceptor;
    CLinkFactory    m_tFactory;
};

}//whalebot
