#ifndef BASELIB_STRING_TOKENS_RESULT
#define BASELIB_STRING_TOKENS_RESULT

#include <vector>
#include <string>

template<class CIter>
class CLimits {
public:
    CLimits();
    CLimits( CIter begin, CIter end );
    CLimits( const CLimits& another );
    bool isEmpty();
    unsigned int size()const;
    
    CIter   m_Begin;
    CIter   m_End;    
};

template<class CIter>
CLimits<CIter>::CLimits()
{}

template<class CIter>
CLimits<CIter>::CLimits( CIter begin, CIter end )
: m_Begin(begin)
, m_End(end)
{}

template<class CIter>
CLimits<CIter>::CLimits( const CLimits<CIter>& another )
: m_Begin(another.m_Begin)
, m_End(another.m_End)
{}

template<class CIter>
bool CLimits<CIter>::isEmpty()
{
    return (m_End > m_Begin);
}

template<class CIter>
unsigned int CLimits<CIter>::size()const
{
    return (m_End - m_Begin);
}

template<class CIter>
class CTokens {
public:
    typedef CLimits<CIter>              CTokenLimit;
    typedef std::vector<CTokenLimit>    CTokenLimitCollection;

    const CTokenLimitCollection&        getLimits()const;

    bool    getPart( size_t start, size_t stop, CTokenLimit& result )const;
    bool    getTail( size_t start, CTokenLimit& result )const;
    bool    getHead( size_t stop, CTokenLimit& result )const;
	
	//unsafe - same functional but no check for errors
    CTokenLimit unsafeGetTail( size_t start )const;    
    CTokenLimit unsafeGetHead( size_t stop )const;
    CTokenLimit unsafeGetPart( size_t start, size_t stop )const;

    void    addToken( CIter begin, CIter end );    
    
private:
    CTokenLimitCollection   m_vLimits;
};

template<class CIter>
const typename CTokens<CIter>::CTokenLimitCollection& CTokens<CIter>::getLimits()const
{
    return m_vLimits;
}

template<class CIter>
bool CTokens<CIter>::getPart( size_t start
                            , size_t stop
                            , typename CTokens<CIter>::CTokenLimit& result )const
{
    if (start >= stop) {
        return false;
    }

    if (start >= m_vLimits.size()) {
        return false;
    }

    if (stop >= m_vLimits.size()) {
        return false;
    }

    result  =   unsafeGetPart(start, stop);

    return true;
}

template<class CIter>
bool CTokens<CIter>::getTail( size_t start
                            , typename CTokens<CIter>::CTokenLimit& result )const
{
    if (start >= m_vLimits.size()) {
        return false;
    }

    result  =   unsafeGetTail(start);

    return true;
}

template<class CIter>
bool CTokens<CIter>::getHead( size_t stop
                            , typename CTokens<CIter>::CTokenLimit& result )const
{
    if (stop >= m_vLimits.size()) {
        return false;
    }

    result  =   unsafeGetHead(stop);

    return true;
}

template<class CIter>
typename CTokens<CIter>::CTokenLimit CTokens<CIter>::unsafeGetTail( size_t start )const
{
    typename CTokens<CIter>::CTokenLimit ret;
    ret.m_Begin =   m_vLimits[start].m_Begin;
    ret.m_End   =   m_vLimits.back().m_End;      
    return ret;
}

template<class CIter>
typename CTokens<CIter>::CTokenLimit CTokens<CIter>::unsafeGetHead( size_t stop )const
{
    typename CTokens<CIter>::CTokenLimit ret;
    ret.m_Begin =   m_vLimits[0].m_Begin;
    ret.m_End   =   m_vLimits[stop].m_End;   

    return ret;
}

template<class CIter>
typename CTokens<CIter>::CTokenLimit CTokens<CIter>::unsafeGetPart( size_t start
                                                                  , size_t stop )const
{
    typename CTokens<CIter>::CTokenLimit ret;
    ret.m_Begin =   m_vLimits[start].m_Begin;
    ret.m_End   =   m_vLimits[stop].m_End;       

    return ret;
}


template<class CIter>
void CTokens<CIter>::addToken( CIter begin, CIter end )
{
    m_vLimits.push_back(CTokenLimit(begin, end));
}

typedef CTokens<const char*>                    CAnsiTokens;
typedef CTokens<std::string::const_iterator>    CStdTokens;

#endif //BASELIB_STRING_TOKENS_RESULT
