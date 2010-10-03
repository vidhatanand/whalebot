#ifndef COOKIE_H
#define COOKIE_H

#include <string>

class CCookie {
public:

    CCookie()
    : Value("")
    , AppliedPath("")
    , ValidDate("")
    {}

    CCookie( const std::string& value
           , const std::string& appliedPath
           , const std::string& validDate )
    : Value(value)
    , AppliedPath(appliedPath)
    , ValidDate(validDate)
    {}

    std::string Value;
    std::string AppliedPath;
    std::string ValidDate;
};

#endif // COOKIE_H
