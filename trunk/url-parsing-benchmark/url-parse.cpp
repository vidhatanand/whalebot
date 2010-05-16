/* 
 * File:   url-parse.cpp
 * Author: hotdox
 *
 * Created on May 12, 2010, 12:02 AM
 */

#include <list>
#include <fstream>

#include <googleurl/src/gurl.h>

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
        GURL    baseUrl(task->m_sBaseUri);

        for (THtmlTask::TUriList::const_iterator uri = currentUris.begin(); uri != currentUris.end(); ++uri) {
            GURL    relativeUrl(baseUrl.Resolve(*uri));
        }
    }    

    return 0;
}

