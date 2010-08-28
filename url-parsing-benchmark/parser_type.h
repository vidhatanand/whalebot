#pragma once

#include <string>

enum eUrlParsers {
        eGoogleParser = 0,
        eNeonParser,
        eHtmlCxxParser,
        eParsersAtAll
};

std::string gGetParserName( eUrlParsers parser );
