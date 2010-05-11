#ifndef OPTIONSPARSER_H
#define OPTIONSPARSER_H
#include "libstring_global.h"
#include "stringparser.h"
#include "optionsbase.h"

class LIBSTRINGSHARED_EXPORT COptionsParser{
public:
                    COptionsParser(COptionsBase &options);
    void            parse(std::string const &str);
private:
    CStringParser   parser;
    COptionsBase    &options;
};

#endif // OPTIONSPARSER_H
