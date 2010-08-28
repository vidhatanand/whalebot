#pragma once

#include <htmlcxx/html/Uri.h>

#include "answer.h"


namespace htmlcxx {

htmlcxx::Uri    gParseBase( const std::string& baseUrl );
TUrlParseResult gParseRel( htmlcxx::Uri& baseUri, const std::string& relativeUrl );

class CParser {
public:
    void parseBase( const std::string& baseUri );
    void parseRel( const std::string& relUri );
private:
    htmlcxx::Uri    m_tUri;
};

}//htmlcxx
