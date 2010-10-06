#include <cstring>

#include <cookie.h>
#include <string_tokenizev2/tokenize_string.h>
#include <string_tokenizev2/trim.h>

static const char           kHighLevelLimit[]           =   ";";
static const char           kLowLevelLimit[]            =   "=";
static const char           kPathPart[]                 =   "path";
static const char           kDomainPart[]               =   "domain";
static const char           kExpiresPart[]              =   "expires";
static const unsigned int   kUsualEqualityPartCounter   =   2;
static const char           kSpaces[]                   =   " \t";

void CCookie::clear()
{
    m_sValidDate.clear();
    m_sValue.clear();
    m_sAppliedPath.clear();
}

void CCookie::fromString( const char* cookieStr )
{
    CAnsiStrGen fullGen(cookieStr);
    CAnsiStrGen highLevelLimitGen(kHighLevelLimit);
    CAnsiStrGen lowLevelLimitGen(kLowLevelLimit);
    CAnsiStrGen spacesGen(kSpaces);

    CAnsiTokens highLevelTokens(tokenizeSequnce(fullGen, highLevelLimitGen));

    if (not highLevelTokens.getLimits().empty()) {
        CAnsiTokens::CTokenLimit    cookiePart(gTrimSequence( highLevelTokens.getLimits().front()
                                                            , spacesGen ));
        m_sValue.assign(cookiePart.m_Begin, cookiePart.m_End);
    }


    for (unsigned int i = 1; i != highLevelTokens.getLimits().size(); ++i) {
        CAnsiTokens::CTokenLimit        lim(gTrimSequence( highLevelTokens.getLimits()[i]
                                                         , spacesGen ));
        CAnsiPartStrGen                 lowLevelGen(lim.m_Begin, lim.m_End);
        CAnsiTokens                     lowLevelTokens(tokenizeSequnce( lowLevelGen
                                                                      , lowLevelLimitGen ));

        const CAnsiTokens::CTokenLimit& firstPart(lowLevelTokens.getLimits().front());

        if ( (kUsualEqualityPartCounter == lowLevelTokens.getLimits().size())
            and
             (strncmp(firstPart.m_Begin, kPathPart, firstPart.size()) == 0) ) {
            //path={uri}
            const CAnsiTokens::CTokenLimit& secondPart(lowLevelTokens.getLimits().back());
            m_sAppliedPath.assign(secondPart.m_Begin, secondPart.size());
        }
        else if ( (kUsualEqualityPartCounter == lowLevelTokens.getLimits().size())
                and
                  (strncmp(firstPart.m_Begin, kDomainPart, firstPart.size()) == 0) ) {
            //*TODO* add domain handling
        }
        else if ( (kUsualEqualityPartCounter == lowLevelTokens.getLimits().size())
                and
                  (strncmp(firstPart.m_Begin, kExpiresPart, firstPart.size()) == 0) ) {

            //expires={Date}
            const CAnsiTokens::CTokenLimit& secondPart(lowLevelTokens.getLimits().back());
            m_sValidDate.assign(secondPart.m_Begin, secondPart.size());
        }
    }
}
