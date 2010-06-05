#pragma once

#include <string>

enum eUrlParsers {
        eMyParser = 0,
        eGoogleParser,
        eNeonParser,
        eHtmlCxxParser,
        eParsersAtAll
};

std::string gGetParserName( eUrlParsers parser );
