/* 
 * File:   OneFetcher.h
 * Author: hotdox
 *
 * Created on February 21, 2009, 9:38 PM
 */

#ifndef _ONEFETCHER_H
#define	_ONEFETCHER_H

#include <iostream>
#include <string>

#include <neon/ne_session.h>
#include <neon/ne_request.h>



const unsigned int external_error(600);

class CLink;
class CHeaderParser;

class COneFetcher{
public:
                    COneFetcher();
    bool    		connect(CLink const &link);
    bool    		request(CLink const &link);
    unsigned int	getHeader(CHeaderParser &header, std::ostream &out);
    bool		    getResponse(std::ostream &out);
                    ~COneFetcher();
private:
    ne_session* m_pConnection;
    std::string m_sLastConnectedHost;
    ne_request* m_pRequest;
};



#endif	/* _ONEFETCHER_H */

