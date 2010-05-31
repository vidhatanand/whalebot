#include "whalebot_parser.h"

using namespace whalebot;

void whalebot::gParseBase( CLinkFactory& factory,
                  CEmptyAcceptor& acceptor,
                  const std::string& baseUrl )
{
    factory.setAcceptor(acceptor);
    factory.pushLink(baseUrl);
    factory.setFrom(acceptor.m_tLink);
}

TUrlParseResult whalebot::gParseRel( CLinkFactory& factory,
                                     CEmptyAcceptor& acceptor,
                                     const std::string& relativeUrl )
{
    factory.pushLink(relativeUrl);
    return TUrlParseResult(acceptor.m_tLink.getServer(), acceptor.m_tLink.getUri());
}

CParser::CParser()
{
    m_tFactory.setAcceptor(m_tAcceptor);
}

void CParser::parseBase( const std::string& baseUri )
{
    gParseBase(m_tFactory, m_tAcceptor, baseUri);
}

void CParser::parseRel( const std::string& relUri )
{
    gParseRel(m_tFactory, m_tAcceptor, relUri);
}

void CEmptyAcceptor::pushLink( const CLink& link )
{
    m_tLink =   link;
}
