#include "answer.h"
#include "parser_type.h"

bool TUrlParseResult::operator == ( const TUrlParseResult& another ) const
{
    return ((m_sHost == another.m_sHost) and (m_sRequest == another.m_sRequest));
}

const TUrlParseResult& TUrlParseResult::operator = (const TUrlParseResult& another )
{
    m_sHost     =   another.m_sHost;
    m_sRequest  =   another.m_sRequest;

    return *this;
}

bool TUrlParseResult::isNull() const
{
    return ((m_sHost.empty()) and (m_sRequest.empty()));
}

TParsersResults gCreateResults()
{
    return TParsersResults(eParsersAtAll, TUrlParseResult());
}
