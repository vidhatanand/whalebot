#ifndef BASELIB_STRING_TOKENS_TOKENIZE
#define BASELIB_STRING_TOKENS_TOKENIZE

#include "token_result.h"
#include "standard_generators.h"

template<class TStrGen, class TDelimGen>
CTokens<typename TStrGen::CIterator> tokenizeSequnce( TStrGen& strGen
                                                    , const TDelimGen& delim )
{
    CTokens<typename TStrGen::CIterator>    ret;
    typename TStrGen::CSymbol               sym;
    typename TStrGen::CIterator             prev(strGen.state());

    while (not strGen.isEmpty()) {

        prev    =   strGen.state();

        while ((strGen.next(sym)) and (gFindInGenerator(delim, sym))) {
            prev    =   strGen.state();
        }

        typename TStrGen::CIterator start(prev);


        prev    =   strGen.state();
        while((strGen.next(sym)) and (not gFindInGenerator(delim, sym))) {
            prev        =   strGen.state();

        }

        typename TStrGen::CIterator stop(prev);

        if (strGen.isEmpty()) {
            stop    =   strGen.state();
        }

        if (start != stop) {
            ret.addToken(start, stop);
        }

    }

    return ret; 
}

#endif //BASELIB_STRING_TOKENS_TOKENIZE
