/* 
 * File:   HeaderParser.h
 * Author: hotdox
 *
 * Created on February 22, 2009, 2:35 PM
 */

#pragma once

#include <string>
#include <map>

#include <neon/ne_request.h>

class CHeaderParser{
public:

    class CIterator {
    public:
                    CIterator();
                    CIterator( const CIterator& another );
                    CIterator( ne_request* request );
                    CIterator( ne_request* request, void* cursor );

        const char* headerKey();
        const char* headerValue();
        void operator ++();
        bool operator !=(const CIterator& r);


    private:

        ne_request* m_pRequest;
        void*       m_pCursor;
        const char* m_pKey;
        const char* m_pValue;
    };
            CHeaderParser();
    void    setRequest( ne_request* request );
    bool    getField(const std::string &field, std::string &value)const;
    bool    getExtension(std::string &ext);
    bool    getCookies(std::string &cookies);

    CIterator begin();
    CIterator end();

    void    clear();
    size_t  size();
private:
    ne_request* m_pRequest;//not owning handling
};

