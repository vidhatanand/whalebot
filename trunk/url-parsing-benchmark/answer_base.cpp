#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "answer_base.h"

void CAnswerBase::addAnswer( unsigned int taskBlockNum,
                             unsigned int taskNum,
                             const THtmlAnswer& answer )
{
    if (m_tBase.size() <= taskBlockNum) {
        m_tBase.resize(taskBlockNum + 1);
    }

    TAnswersList&   curList(m_tBase[taskBlockNum]);

    if (curList.size() <= taskNum) {
        curList.resize(taskNum + 1);
    }

    curList[taskNum]    =   answer;

}

const THtmlAnswer* const CAnswerBase::getAnswer(    unsigned int taskBlockNum,
                                                    unsigned int taskNum ) const {

    if (m_tBase.size() <= taskBlockNum) {
        return 0;
    }

    const TAnswersList&   curList(m_tBase[taskBlockNum]);

    if (curList.size() <= taskNum) {
        return 0;
    }

    return &curList[taskNum];
}

bool CAnswerBase::read( const std::string &path )
{
    std::ifstream                   ifs(path.c_str());

    if (not ifs.is_open()) {
        return false;
    }

    boost::archive::text_iarchive   ia(ifs);
    boost::serialization::serialize(ia, *this, 1);

    return true;
}

bool CAnswerBase::write( const std::string &path )
{
    std::ofstream                   ofs(path.c_str());

    if (not ofs.is_open()) {
        return false;
    }
    boost::archive::text_oarchive   oa(ofs);
    boost::serialization::serialize(oa, *this, 1);

    return true;
}
