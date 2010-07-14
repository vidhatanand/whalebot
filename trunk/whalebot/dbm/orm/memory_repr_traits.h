#pragma once

#include <vector>
#include <string>
#include <set>
#include <map>
#include <list>
#include <deque>

namespace korm {

const unsigned int kPodType     =   1;
const unsigned int kNonPodType  =   0;

template<class T>
struct CMemoryReprTraits {
    enum { eIsPod = kPodType };
};

template<class T>
struct CMemoryReprTraits<T*> {
    enum { eIsPod = kNonPodType };
};

template<class T>
struct CMemoryReprTraits<std::vector<T> > {
    enum { eIsPod = CMemoryReprTraits<T>::eIsPod };
};

template<class T>
struct CMemoryReprTraits<std::list<T> > {
    enum { eIsPod = kNonPodType };
};

template<class KT, class VT>
struct CMemoryReprTraits<std::map<KT,VT> > {
    enum { eIsPod = kNonPodType };
};

template<class T>
struct CMemoryReprTraits<std::set<T> > {
    enum { eIsPod = kNonPodType };
};

template<class T>
struct CMemoryReprTraits<std::deque<T> > {
    enum { eIsPod = kNonPodType };
};

template<typename _CharT, typename _Traits, typename _Alloc>
struct CMemoryReprTraits<std::basic_string<_CharT, _Traits, _Alloc> > {
    enum { eIsPod = kPodType };
};

template<class FT, class ST>
struct CMemoryReprTraits<std::pair<FT, ST> > {
    enum { eIsPod = kNonPodType };
};

}//korm - kyoto orm
