#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <parserbase.h>

#include <header_parser.h>

CHeaderParser::CHeaderParser()
: m_pRequest(0)
{}

void CHeaderParser::setRequest( ne_request* request )
{
    m_pRequest  =   request;
}

bool CHeaderParser::getField(const std::string &field, std::string &value)const
{
    bool        ret(false);
    const char* result(ne_get_response_header(m_pRequest, field.c_str()));

    if (0 != result) {
        ret     =   true;
        value   =   result;
    }

    return ret;
}

bool CHeaderParser::getCookies(std::string &cookies)
{
    return getField("Set-Cookie",cookies);
}

bool CHeaderParser::getExtension(std::string &ext)
{
    bool    ret(true);
    
    if(!getField("Content-Type", ext)) {
        return false;
    }

    CParserBase parser(";");
    CParserBase inner_parser("/");

    parser.parseString(ext);
    ret =   parser.getElement(0, ext);
    inner_parser.parseString(ext);
    inner_parser.getTail(1, ext);
    return ret;
}

CHeaderParser::CIterator CHeaderParser::begin()
{
    return CIterator(m_pRequest);
}

CHeaderParser::CIterator CHeaderParser::end()
{
    return CIterator();
}

CHeaderParser::CIterator::CIterator()
: m_pRequest(0)
, m_pCursor(0)
, m_pKey(0)
, m_pValue(0)
{}

CHeaderParser::CIterator::CIterator( const CIterator& another )
: m_pRequest(another.m_pRequest)
, m_pCursor(another.m_pCursor)
, m_pKey(another.m_pKey)
, m_pValue(another.m_pValue)
{}

CHeaderParser::CIterator::CIterator( ne_request* request )
: m_pRequest(request)
, m_pCursor(0)
, m_pKey(0)
, m_pValue(0)
{
    m_pCursor   =   ne_response_header_iterate(m_pRequest, m_pCursor, &m_pKey, &m_pValue);
}

const char* CHeaderParser::CIterator::headerKey()
{
    return m_pKey;
}

const char* CHeaderParser::CIterator::headerValue()
{
    return m_pValue;
}

void CHeaderParser::CIterator::operator ++()
{
    m_pCursor   =   ne_response_header_iterate(m_pRequest, m_pCursor, &m_pKey, &m_pValue);
}

bool CHeaderParser::CIterator::operator !=(const CIterator& r)
{
    return m_pCursor != r.m_pCursor;
}



