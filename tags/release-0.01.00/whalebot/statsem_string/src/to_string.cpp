#include <fstream>
#include <to_string.h>

template<>
std::string toString(std::string const &x){
    return x;
}

void trim(std::string &where, std::string const &what){
    std::string::iterator           begin(inner_cut(where.begin(), where.end(), what));
    std::string::iterator           end(inner_cut(where.rbegin(), where.rend(), what).base());
    if(begin < end){
        where.assign(begin, end);
    }else{
        where.clear();
    }
}


template<>
bool fromString(std::string const &x,std::string  &y){
    y = x;
    return true;
}

void copyStream(std::istream &from , std::ostream &to){
    to << from.rdbuf();
}

