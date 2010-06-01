#include "neon_parser.h"


using namespace neon;

ne_uri neon::gParseBase( const std::string& baseUrl )
{
    ne_uri base;
    char*   decoded(ne_path_unescape(baseUrl.c_str()));
    ne_uri_parse(decoded, &base);
    free(decoded);

    return base;
}

TUrlParseResult neon::gParseRel( ne_uri* baseUrl, const std::string& relativeUrl )
{
    ne_uri  relUrl;
    char*   decoded(ne_path_unescape(relativeUrl.c_str()));
    ne_uri_parse(decoded, &relUrl);
    free(decoded);

    ne_uri  resUrl;
    ne_uri_resolve(baseUrl, &relUrl, &resUrl);

    TUrlParseResult ret(resUrl.host, resUrl.path);

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


