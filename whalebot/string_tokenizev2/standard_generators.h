#ifndef BASELIB_STRING_TOKEN_STANDARD_GENERATORS
#define BASELIB_STRING_TOKEN_STANDARD_GENERATORS

#include <string>
#include <iostream>


//! /breif
//!     Generator interface for c strings
template<class T>
class CCStringGenerator {
public:

    typedef const T* CIterator;
    typedef T        CSymbol;

    CCStringGenerator( CIterator pCStr );
    CCStringGenerator( const CCStringGenerator& another );
    CIterator state()const;
    bool next( T& c );
    bool isEmpty()const;
private:
    CIterator   m_pStr;
};

template<class T>
CCStringGenerator<T>::CCStringGenerator( CCStringGenerator<T>::CIterator pCStr )
: m_pStr(pCStr)
{}

template<class T>
CCStringGenerator<T>::CCStringGenerator( const CCStringGenerator<T>& another )
: m_pStr(another.state())
{}

template<class T>
typename CCStringGenerator<T>::CIterator CCStringGenerator<T>::state()const
{
    return m_pStr;
}

template<class T>
bool CCStringGenerator<T>::next( T& c )
{
    bool    ret(false);
    if (not isEmpty()) {
        c   =   *m_pStr;
        ret =   true;
        ++m_pStr;
    }

    return ret;
}

template<class T>
bool CCStringGenerator<T>::isEmpty()const
{
    return (*m_pStr == 0);
}


//! /breif
//!     Generator interface for std strings

template<class CIter, class T = typename std::iterator_traits<CIter>::value_type>
class CPairGenerator {
public:

    typedef CIter   CIterator;
    typedef T       CSymbol;

    CPairGenerator( CIterator begin, CIterator end );
    CPairGenerator( const CPairGenerator& another );
    CIterator state()const;
    bool next( T& c );
    bool isEmpty()const;

private:

    CIterator   m_Pos;
    CIterator   m_End;
};

template<class CIter, class T>
CPairGenerator<CIter, T>::CPairGenerator( CPairGenerator<CIter, T>::CIterator begin,
                                          CPairGenerator<CIter, T>::CIterator end )
: m_Pos(begin)
, m_End(end)
{}

template<class CIter, class T>
CPairGenerator<CIter, T>::CPairGenerator( const CPairGenerator<CIter, T>& another )
: m_Pos(another.m_Pos)
, m_End(another.m_End)
{}

template<class CIter, class T>
typename CPairGenerator<CIter, T>::CIterator CPairGenerator<CIter, T>::state()const
{
    return m_Pos;
}

template<class CIter, class T>
bool CPairGenerator<CIter, T>::next( T& c )
{
    bool    ret(false);

    if (not isEmpty()) {
        c   =   *m_Pos;
        ret =   true;
        ++m_Pos;
    }


    return ret;
}

template<class CIter, class T>
bool CPairGenerator<CIter, T>::isEmpty()const
{
    return (m_Pos == m_End);
}

// ! \brief describe geneartor for single symbol
template<class T>
class TConstGenerator {
public:
    typedef bool CIterator;
    typedef T    CSymbol;

    TConstGenerator( const T symbol );
    TConstGenerator( const TConstGenerator& another );
    CIterator state()const;
    bool next( T& c );


private:
    bool    m_state;
    T       m_symbol;
};

template<class T>
TConstGenerator<T>::TConstGenerator( const T symbol )
: m_state(true)
, m_symbol(symbol)
{}

template<class T>
TConstGenerator<T>::TConstGenerator( const TConstGenerator<T>& another )
: m_state(another.m_state)
, m_symbol(another.m_symbol)
{}

template<class T>
bool TConstGenerator<T>::next( T& c )
{
    if (m_state) {
        c       = m_symbol;
        m_state = false;
        return true;
    }
    else {
        return false;
    }
}

template<class T>
typename TConstGenerator<T>::CIterator TConstGenerator<T>::state()const
{
    return m_state;
}

typedef CCStringGenerator<char>                     CAnsiStrGen;
typedef CPairGenerator<std::string::const_iterator> CStdStrGen;
typedef CPairGenerator<const char*>                 CAnsiPartStrGen;
typedef TConstGenerator<char>                       CConstCharGen;

template<class TGen>
bool gFindInGenerator( const TGen& generator, typename TGen::CSymbol c )
{
    TGen                    gen(generator);
    typename TGen::CSymbol  sym;
    bool                    ret(false);
    while ((not ret) and (gen.next(sym))) {
        ret =   (sym == c);
    }
    return ret;
}

template<class TGen>
void gPrintGenerator( const TGen& generator, std::ostream& ostr )
{
    TGen                    gen(generator);
    typename TGen::TSymbol  sym;
    while (gen.next(sym)) {
        ostr << sym;
    }
}


#endif //BASELIB_STRING_TOKEN_STANDARD_GENERATORS
