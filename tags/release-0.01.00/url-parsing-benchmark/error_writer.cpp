#include "error_writer.h"

TErrorWriter::TErrorWriter()
: m_sBaseUri("")
, m_pOutputFile(0)
{}

TErrorWriter::TErrorWriter( const TErrorWriter& another )
: m_sBaseUri(another.m_sBaseUri)
, m_pOutputFile(another.m_pOutputFile)
{}

bool TErrorWriter::init( const std::string& filePath )
{
    m_pOutputFile   =   new std::ofstream(filePath.c_str(), std::ios_base::out | std::ios_base::trunc);

    return m_pOutputFile->is_open();
}

void TErrorWriter::writeErrorUri( const std::string& baseUri,
                                  const std::string& relUri )
{
    if (m_sBaseUri != baseUri) {
        (*m_pOutputFile) << baseUri << std::endl;
        m_sBaseUri  =   baseUri;
    }

   (*m_pOutputFile) << '\t' << relUri << std::endl;
}

TErrorWriter::~TErrorWriter()
{
    delete m_pOutputFile;
}
