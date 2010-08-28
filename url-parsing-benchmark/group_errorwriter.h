#pragma once

#include <vector>

#include "error_writer.h"
#include "parser_type.h"

class TGroupErrorWriter
{
public:
            TGroupErrorWriter();
    bool    init();
    void    writeError( eUrlParsers mistakedParser,
                       const std::string& baseUri,
                       const std::string& relUri );
private:
    std::vector<TErrorWriter>   m_vWriters;
};
