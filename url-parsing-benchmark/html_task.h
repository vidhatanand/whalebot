#pragma once

#include <list>
#include <string>




struct THtmlTask {
    std::string m_sBaseUri;
    typedef std::list<std::string>  TUriList;
    TUriList    m_lUris;
};

typedef std::list<THtmlTask> THtmlTaskList;


void gReadTasksFromFile( std::ifstream& file, THtmlTaskList& tasks );
