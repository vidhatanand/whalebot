#pragma once

#include <string>
#include <vector>

#include <boost/serialization/string.hpp>

class TUrlParseResult {
public:

    TUrlParseResult()
    : m_sHost("")
    , m_sRequest("")
    {}

    TUrlParseResult(const std::string& host, const std::string& request)
    : m_sHost(host)
    , m_sRequest(request)
    {}

    TUrlParseResult( const TUrlParseResult& another )
    : m_sHost(another.m_sHost)
    , m_sRequest(another.m_sRequest)
    {}

    bool operator == ( const TUrlParseResult& another ) const;

    bool isNull() const;

    std::string m_sHost;
    std::string m_sRequest;
};

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive& ar, TUrlParseResult& res, const unsigned int version)
{

    ar & res.m_sHost;
    ar & res.m_sRequest;
}

} // namespace serialization
} // namespace boost


typedef std::vector<TUrlParseResult> TParsersResults;

TParsersResults gCreateResults();
