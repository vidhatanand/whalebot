#include "htmlcxx_parser.h"

using namespace htmlcxx;

htmlcxx::Uri htmlcxx::gParseBase( const std::string& baseUrl )
{
    return htmlcxx::Uri(htmlcxx::Uri::decode(baseUrl));
}

TUrlParseResult htmlcxx::gParseRel( htmlcxx::Uri& baseUri, const std::string& relativeUrl )
{
    htmlcxx::Uri    tmp(htmlcxx::Uri::decode(relativeUrl));
    htmlcxx::Uri    relativeUri(tmp.absolute(baseUri));

    return TUrlParseResult(
                            relativeUri.hostname(),
                            relativeUri.path() + ( relativeUri.existsQuery()
                                                        ? "?" + relativeUri.query()
                                                        : "" )
                           );
}


void CParser::parseBase( const std::string& baseUri )
{
    m_tUri  =   gParseBase(baseUri);
}

void CParser::parseRel( const std::string& relUri )
{
    gParseRel(m_tUri, relUri);
}
