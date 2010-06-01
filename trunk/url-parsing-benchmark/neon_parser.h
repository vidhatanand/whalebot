#pragma once

#include <cstdlib>

#include <string>
#include <neon/ne_uri.h>

#include "answer.h"

namespace neon {


ne_uri gParseBase( const std::string& baseUrl );

TUrlParseResult gParseRel( ne_uri* baseUrl, const std::string& relativeUrl );

class CParser {
public:

    CParser();

    void parseBase( const std::string& baseUri );

    void parseRel( const std::string& relUri );

    ~CParser();

private:
    ne_uri m_tUri;
};
} //neon
