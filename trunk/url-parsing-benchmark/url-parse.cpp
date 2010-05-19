/* 
 * File:   url-parse.cpp
 * Author: hotdox
 *
 * Created on May 12, 2010, 12:02 AM
 */

#include <list>
#include <fstream>

#include <googleurl/src/gurl.h>
#include "../whalebot/webspider/include/link_factory.h"


class TEmptyAcceptor : public IAcceptor {
public:
    void    pushLink(const CLink& link)
    {
        m_tLink =   link;
        return;
    }

    CLink   m_tLink;

};


struct THtmlTask {
    std::string m_sBaseUri;
    typedef std::list<std::string>  TUriList;
    TUriList    m_lUris;
};

typedef std::list<THtmlTask> THtmlTaskList;

const char          kDelimiter          =   '\t';
const std::string   kEmptyTabbedString  =   std::string(1, kDelimiter);

void readTasksFromFile(std::ifstream& file, THtmlTaskList& tasks)
{
    
    std::string     tmpStr("");
    bool            continueRead(true);


    //skip tabbed
    while ((continueRead) and (std::getline(file, tmpStr))) {
        if (not tmpStr.empty()) {
            continueRead   =   (*tmpStr.begin() == kDelimiter);
        }
    }
    
    THtmlTask   task;
    
    task.m_sBaseUri =   tmpStr;
    
    
    while (std::getline(file, tmpStr)) {

        if ((tmpStr.empty()) or (tmpStr == kEmptyTabbedString)) {
            continue;
        }

        if (*tmpStr.begin() != kDelimiter) {
            tasks.push_back(task);

            task.m_sBaseUri =   tmpStr;
            task.m_lUris.clear();
            continue;
        }

        tmpStr.erase(0, 1);

        task.m_lUris.push_back(tmpStr);
    }
}



int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << argv[0] << " \"link file\"" << std::endl;
        return 1;
    }

    std::ifstream   file(argv[1]);

    THtmlTaskList   tasks;

    readTasksFromFile(file, tasks);


    for (THtmlTaskList::const_iterator task = tasks.begin(); task != tasks.end(); ++task) {
        const THtmlTask::TUriList&  currentUris(task->m_lUris);
        GURL                        baseUrl(task->m_sBaseUri);
        CLinkFactory                linkFactory;
        TEmptyAcceptor              acceptor;

        linkFactory.setAcceptor(acceptor);
        linkFactory.pushLink(task->m_sBaseUri);

        linkFactory.setFrom(acceptor.m_tLink);
        

        for (THtmlTask::TUriList::const_iterator uri = currentUris.begin(); uri != currentUris.end(); ++uri) {

            GURL    relativeUrl(baseUrl.Resolve(*uri));

            std::string gurlHost(relativeUrl.host());
            std::string gurlRequest(relativeUrl.PathForRequest());



            linkFactory.pushLink(*uri);

            std::string myHost(acceptor.m_tLink.getServer());
            std::string myRequest(acceptor.m_tLink.getUri());

            if ((myHost != gurlHost) or (myRequest != gurlRequest)) {
                std::cout << std::endl;
                std::cout << "------------------------------------" << std::endl;
                std::cout << std::endl;

                std::cout << "my   : " << std::endl;
                std::cout << "      host  : " << myHost << std::endl;
                std::cout << "      query : " << myRequest << std::endl;
                std::cout << std::endl;
                std::cout << "gurl : " << std::endl;
                std::cout << "      host  : " << gurlHost << std::endl;
                std::cout << "      query : " << gurlRequest << std::endl;
            }
        }
    }    

    return 0;
}

