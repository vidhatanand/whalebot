

#include "equivalence.h"

TEquivalenceRelation gFindRelated( const TParsersResults& results )
{

    typedef std::vector<bool>               TIncludedParser;


    TEquivalenceRelation    ret;
    unsigned int            parsersCount(results.size());
    TIncludedParser         included(static_cast<unsigned int>(parsersCount), false);

    for (unsigned int i = 0; i < parsersCount; ++i) {

        if (included[i]) {
            continue;
        }

        const TUrlParseResult&  currentResult(results[i]);

        if (currentResult.isNull()) {
            continue;
        }

        TEquivalenceClass   currentClass(1, static_cast<eUrlParsers>(i));

        for (unsigned int j = i + 1; j < parsersCount; ++j) {

            if (included[j]) {
                continue;
            }

            const TUrlParseResult&  secondResult(results[j]);

            if (secondResult.isNull()) {
                continue;
            }

            if (secondResult == currentResult) {
                included[j] =   true;
                currentClass.push_back(static_cast<eUrlParsers>(j));
            }
        }

        ret.push_back(currentClass);
    }

    return ret;
}
