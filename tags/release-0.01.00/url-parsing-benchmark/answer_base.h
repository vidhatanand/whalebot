#pragma once

#include <boost/serialization/vector.hpp>

#include <string>
#include <vector>

#include "answer.h"

typedef TUrlParseResult             THtmlAnswer;
typedef std::vector<THtmlAnswer>    TAnswersList;
typedef std::vector<TAnswersList>   TAnswersBase;


class CAnswerBase {
public:

    void addAnswer( unsigned int taskBlockNum,
                    unsigned int taskNum,
                    const THtmlAnswer& answer );

    const THtmlAnswer* const getAnswer( unsigned int taskBlockNum,
                                        unsigned int taskNum ) const;

    bool read( const std::string& path );

    bool write( const std::string& path );

    TAnswersBase m_tBase;
};



namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive& ar, CAnswerBase& base, const unsigned int version)
{
    serialize(ar, base.m_tBase, version);
}

} // namespace serialization
} // namespace boost


