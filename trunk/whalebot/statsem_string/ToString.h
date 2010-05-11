/*
 * ToString.h
 *
 *  Created on: 24.10.2008
 *      Author: hotdox
 */

#ifndef TOSTRING_H_
#define TOSTRING_H_
#include <string>
#include <utility>
#include <sstream>
#include <vector>
#include <iostream>
#include "libstring_global.h"


template<class T>
std::string toString(const T &x){
    std::stringstream ss;
    ss << x;
    return ss.str();
}
template<>
std::string LIBSTRINGSHARED_EXPORT toString(std::string const &x);


template<class T>
bool fromString(std::string const &x,T &y){
	bool				ret(false);
	T 					out;
    std::istringstream	stream(x);
    ret	=	stream>>out;

    if(ret){
    	y	=	out;
    }
    return ret;
}

template<>
bool LIBSTRINGSHARED_EXPORT fromString(std::string const &x,std::string  &y);

template<class si>
si inner_cut(si begin, si end, std::string const &symbols){
    while((begin != end)&&(symbols.find(*begin) != std::string::npos)){
        ++begin;
    }
    return begin;
}

void LIBSTRINGSHARED_EXPORT trim(std::string &where, std::string const &what);
void LIBSTRINGSHARED_EXPORT copyStream(std::istream &from , std::ostream &to);

#endif /* TOSTRING_H_ */
