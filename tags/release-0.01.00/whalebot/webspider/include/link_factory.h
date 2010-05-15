/* 
 * File:   LinkFactory.h
 * Author: hotdox
 *
 * Created on September 19, 2009, 2:37 PM
 */

#ifndef _LINKFACTORY_H
#define	_LINKFACTORY_H

#include <string>

#include "ilink_factory.h"



class IAcceptor;


class CLinkFactory : public ILinkFactory {
public:
                CLinkFactory();
    void        pushLink(std::string link);
    void        setFrom(CLink const &uri);
    void        setAcceptor(IAcceptor &acceptor);

private:
    void static cutSticks(std::string &link);
    void static cutCutId(std::string &link);
    void static cutHtml(std::string &link);
    void        cutCookie(std::string &link)const;
    void        fullCut(std::string &link)const;
    void static normolizeQeury(std::string &link);
    CLink       normolizeLink(std::string &link)const;
private:
    CLink       m_parentLink;
    IAcceptor   *m_pAcceptor;
};



#endif	/* _LINKFACTORY_H */

