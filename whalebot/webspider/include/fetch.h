#ifndef WHALEBOT_FETCH_H
#define WHALEBOT_FETCH_H

#include <string>

#include <neon/ne_request.h>
#include <cookie.h>

int gFetch( ne_request* request
          //, request url
          , const std::string& filePath//path to save file if it is too big without extension
          //, fetching options
          //header part
          , TCookieJar& responseCookie
          , std::string& redirectionUrl
          , std::string& fileType
          //response
          , std::string& responseBody
          //,detected links);
          , unsigned int* bytesFetched );

#endif // FETCH_H
