#pragma once

#include <string>
#include <fstream>

class TErrorWriter {
public:
            TErrorWriter();
            TErrorWriter( const TErrorWriter& another );
    bool    init( const std::string& filePath );
    void    writeErrorUri( const std::string& baseUri,
                           const std::string& relUri );
            ~TErrorWriter();
private:
    std::string     m_sBaseUri;
    std::ofstream*  m_pOutputFile;
};


