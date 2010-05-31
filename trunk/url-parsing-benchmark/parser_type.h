#pragma once

#include <string>

enum eUrlParsers {
        eMyParser = 0,
        eGoogleParser,
        eNeonParser,
        eHtmlCxxParser,
        eRegExpParser,
        eParsersAtAll
};

std::string gGetParserName( eUrlParsers parser );
