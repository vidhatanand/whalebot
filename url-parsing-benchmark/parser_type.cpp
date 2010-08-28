#include "parser_type.h"


std::string gGetParserName( eUrlParsers parser )
{
#define PARSER_CASE(p) \
    case (e##p) : \
                return #p;

        switch (parser) {
            PARSER_CASE(GoogleParser);
            PARSER_CASE(NeonParser);
            PARSER_CASE(HtmlCxxParser);
        default:
            return "unknown";
        }
}
