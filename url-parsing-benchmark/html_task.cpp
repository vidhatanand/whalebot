#include <fstream>

#include <htmlcxx/html/utils.h>

#include "../whalebot/webspider/include/prefix.h"

#include "html_task.h"


static const char           kDelimiter          =   '\t';
static const std::string    kEmptyTabbedString  =   std::string(1, kDelimiter);

void gReadTasksFromFile( std::ifstream& file, THtmlTaskList& tasks )
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

            task.m_sBaseUri =   htmlcxx::HTML::decode_entities(tmpStr);
            task.m_lUris.clear();
            continue;
        }

        tmpStr.erase(0, 1);

        if ((prefix::isMail(tmpStr)) or (prefix::isJavaScript(tmpStr))) {
            continue;
        }

        task.m_lUris.push_back(htmlcxx::HTML::decode_entities(tmpStr));
    }

    tasks.push_back(task);

    //remove empty tasks
    THtmlTaskList::iterator taskIter(tasks.begin());
    while (taskIter != tasks.end()) {
        if (taskIter->m_lUris.empty()) {
            taskIter    =   tasks.erase(taskIter);
        }
        else {
            ++taskIter;
        }
    }

    //remove duplicates

}

