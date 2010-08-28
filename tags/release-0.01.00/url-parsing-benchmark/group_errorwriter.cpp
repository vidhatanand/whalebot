#include "group_errorwriter.h"

TGroupErrorWriter::TGroupErrorWriter()
: m_vWriters(eParsersAtAll, TErrorWriter())
{
}

bool TGroupErrorWriter::init()
{
    bool    ret(true);

    for (unsigned int i = 0; i != m_vWriters.size(); ++i) {
        TErrorWriter&   curWriter(m_vWriters[i]);
        std::string     filePath(gGetParserName(static_cast<eUrlParsers>(i)));
        filePath    +=  "_errors.txt";

        ret =   ret & curWriter.init(filePath);
    }

    return ret;
}

void TGroupErrorWriter::writeError( eUrlParsers mistakedParser,
                                    const std::string& baseUri,
                                    const std::string& relUri )
{
    TErrorWriter&   curWriter(m_vWriters[static_cast<unsigned int>(mistakedParser)]);
    curWriter.writeErrorUri(baseUri, relUri);
}
