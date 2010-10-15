#include <neon/ne_string.h>

#include <fstream>

#include <fetch.h>
#include <http_const.h>


static const char   kHttpHeaderSetCookieField[]     =   "Set-cookie";
static const char   kHttpHeaderContentTypeField[]   =   "Content-Type";
static const char   kHttpHeaderLocationField[]      =   "Location";

static const unsigned int   kMaxPageSize            =   10 * 1024;//10 Mb


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
          //,detected links
          , unsigned int* bytesFetched )
{
    //*TODO* add automatic request cleaner
    //*TODO* reserve kMaxRequestBytes
    int requestResult(ne_begin_request(request));

    if ((NE_OK != requestResult) and (NE_REDIRECT == requestResult)) {
        ne_request_destroy(request);
        return ne_get_status(request)->code;
    }

    //parse header
    responseCookie.clear();

    void*       headerCursor(0);
    const char* headerKey(0);
    const char* headerValue(0);

    do {

        ne_response_header_iterate(request, headerCursor, &headerKey, &headerValue);

        if (0 == ne_strcasecmp(kHttpHeaderSetCookieField, headerKey)) {
            responseCookie.push_back(CCookie(headerValue));
        }
        else if (0 == ne_strcasecmp(kHttpHeaderLocationField, headerKey)) {
            redirectionUrl  =   headerValue;
        }
        else if (0 == ne_strcasecmp(kHttpHeaderContentTypeField, headerKey)) {
            fileType    =   headerValue;
        }
    }
    while (0 != headerCursor);

    //detect extension and need in parsing
//    if ((not isSaveFiles) and (kHtmlMimeType != mimiType)) {
//        ne_request_destroy(request);
//        return ne_get_status(request)->code;
//    }


    responseBody.clear();

    static char     kReadBuffer[kDefaultReadBufferSizeInBytes];
    unsigned int    bytesFetchedInternal(0);
    ssize_t         readSize;
    std::ofstream*  outputFile(0);


    //*TODO* add some kind of smart_ptr
    //check for extension
    //if we do not parse it => do not store it in memory => create ofstream

    while ((readSize = ne_read_response_block( request
                                             , kReadBuffer
                                             , kDefaultReadBufferSizeInBytes )) > 0) {


        if (0 != outputFile) {
            //we write in file
            outputFile->write(kReadBuffer, readSize);
        }
        else {
            //we still write to str
            if (bytesFetchedInternal + readSize > kMaxPageSize) {
                //file is so big what we do not save it in memory
                //write to disc
                outputFile  =   new std::ofstream(filePath.c_str(), std::ios_base::binary);
                outputFile->write(responseBody.c_str(), responseBody.size());

                //true way to really free memory
                //stdlibc++ think that we may need allocated memory later
                //but this memory is useless
                std::string emptyString("");
                responseBody.swap(emptyString);
            }
            else {
                responseBody.append(kReadBuffer, readSize);
            }
        }

        bytesFetchedInternal    +=  readSize;
    }

    if (0 != outputFile) {
        outputFile->close();
        delete outputFile;
    }

    if (readSize == 0) {
        ne_end_request(request);
    }

    ne_request_destroy(request);

    if (0 != bytesFetchedInternal) {
        *bytesFetched   =   bytesFetchedInternal;
    }
    //parse html for links

    if (false == responseBody.empty()) {

    }

    return ne_get_status(request)->code;
}
