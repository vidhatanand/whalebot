/* 
 * File:   url-parse.cpp
 * Author: hotdox
 *
 * Created on May 12, 2010, 12:02 AM
 */

#include <fstream>
#include <LinkFactory.h>
#include <googleurl/src/gurl.h>

/*
 * 
 */
int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << argv[0] << " \"link file\"" << std::endl;
        return 1;
    }
    std::ifstream   file(argv[1]);
    std::string     tmpStr("");
    bool            _continue(true);

    //skip tabbed
    while ((_continue) and (std::getline(file, tmpStr))) {
        if (not tmpStr.empty()) {
            _continue   =   (*tmpStr.begin() == '\t');
        }
    }


    std::string tmpStr3("http://");
    tmpStr3 +=  tmpStr;
    GURL    baseUrl(tmpStr3);

    if (not baseUrl.is_valid()) {
        std::cout << "'" << tmpStr3 << "' is invalid base url" << std::endl;
    }

    std::cout << "----------------------" <<std::endl;




    while (std::getline(file, tmpStr)) {

        if ((tmpStr.empty()) or (tmpStr == "\t")) {
            continue;
        }

        if (*tmpStr.begin() != '\t') {


            tmpStr3 =   "http://";
            tmpStr3 +=  tmpStr;


            GURL    anotherBaseUrl(tmpStr3);
            baseUrl.Swap(&anotherBaseUrl);

            if (not baseUrl.is_valid()) {
                std::cout << "'" << tmpStr3 << "' is invalid base url" << std::endl;
            }

            std::cout << "----------------------" <<std::endl;

            continue;
        }

        std::string tmpStr2(tmpStr.substr(1));
        
        GURL    relativeUrl(baseUrl.Resolve(tmpStr2));

        if ((not relativeUrl.is_valid()) or (relativeUrl.is_empty())) {
            std::cout << "'" << tmpStr2 << "' is invalid relative url" << std::endl;
        }

        std::cout << relativeUrl.host() << std::endl;
        std::cout << relativeUrl.PathForRequest() << std::endl;

        std::cout << "----------" <<std::endl;


    }


    

    return 0;
}

