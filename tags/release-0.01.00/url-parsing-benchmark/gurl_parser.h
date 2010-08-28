#pragma once

#include <string>

#include <googleurl/src/gurl.h>
#include <htmlcxx/html/Uri.h>

#include "answer.h"

namespace gurl {

GURL  gParseBase( const std::string& baseUrl );

TUrlParseResult gParseRel( GURL& baseUrl, const std::string& relativeUrl );

class CParser {
public:
    void parseBase( const std::string& baseUri );

    void parseRel( const std::string& relUri );

private:
    GURL    m_tUri;
};

}//gurl

