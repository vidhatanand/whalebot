#ifndef BASELIB_STRING_TOKEN_TRIM
#define BASELIB_STRING_TOKEN_TRIM

#include "standard_generators.h"
#include "token_result.h"

template<class CIter, class CGen>
CLimits<CIter> gTrimSequence( const CLimits<CIter>& sequence, const CGen& spaces )
{
    CLimits<CIter>  ret(sequence);
    while ((ret.m_Begin <= ret.m_End) and (gFindInGenerator(spaces, *ret.m_Begin))) {
        ++ret.m_Begin;
    }

    while ((ret.m_Begin < ret.m_End) and (gFindInGenerator(spaces, *(ret.m_End - 1)))) {
        --ret.m_End;
    }

    return ret;
}

#endif //BASELIB_STRING_TOKEN_TRIM
