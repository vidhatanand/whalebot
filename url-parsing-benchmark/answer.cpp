#include "answer.h"
#include "parser_type.h"

bool TUrlParseResult::operator == ( const TUrlParseResult& another ) const
{
    return ((m_sHost == another.m_sHost) and (m_sRequest == another.m_sRequest));
}

bool TUrlParseResult::isNull() const
{
    return ((m_sHost.empty()) and (m_sRequest.empty()));
}

TParsersResults gCreateResults()
{
    return TParsersResults(eParsersAtAll, TUrlParseResult());
}
