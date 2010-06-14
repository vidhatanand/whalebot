#include "neon_parser.h"


using namespace neon;

ne_uri neon::gParseBase( const std::string& baseUrl )
{
    ne_uri base;
    ne_uri_parse(baseUrl.c_str(), &base);
    return base;
}

TUrlParseResult neon::gParseRel( ne_uri* baseUrl, const std::string& relativeUrl )
{
    ne_uri  relUrl;
    ne_uri_parse(relativeUrl.c_str(), &relUrl);
    

    ne_uri  resUrl;
    ne_uri_resolve(baseUrl, &relUrl, &resUrl);

    TUrlParseResult ret;

    if (0 != resUrl.host) {
        ret.m_sHost =   resUrl.host;
    }

    if (0 != resUrl.path) {
        ret.m_sRequest  =   resUrl.path;
    }

    if (0 != resUrl.query) {
        ret.m_sRequest.append(1, '?');
        ret.m_sRequest.append(resUrl.query);
    }

    ne_uri_free(&resUrl);
    ne_uri_free(&relUrl);

    return ret;
}



CParser::CParser()
{
    ne_uri_parse("www.example.com", &m_tUri);
}

void CParser::parseBase( const std::string& baseUri )
{
    ne_uri_free(&m_tUri);
    m_tUri  =   gParseBase(baseUri);
}

void CParser::parseRel( const std::string& relUri )
{
    gParseRel(&m_tUri, relUri);
}

CParser::~CParser()
{
    ne_uri_free(&m_tUri);
}


