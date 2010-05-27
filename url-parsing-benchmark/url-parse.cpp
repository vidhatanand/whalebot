/* 
 * File:   url-parse.cpp
 * Author: hotdox
 *
 * Created on May 12, 2010, 12:02 AM
 */

#include <list>
#include <vector>
#include <fstream>

#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <googleurl/src/gurl.h>
#include <htmlcxx/html/Uri.h>

#include "../whalebot/webspider/include/link_factory.h"

using namespace boost::posix_time;


class TEmptyAcceptor : public IAcceptor {
public:
    void    pushLink( const CLink& link )
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

void readTasksFromFile( std::ifstream& file, THtmlTaskList& tasks )
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

}

enum eUrlParsers {
	eMyParser = 0,
	eGoogleParser,
	eNeonParser,
        eHtmlCxxParser,
        eRegExpParser,
	eParsersAtAll	
};

static std::string getParserName( eUrlParsers parser )
{
#define PARSER_CASE(p) \
    case (e##p) : \
                return #p;

        switch (parser) {
            PARSER_CASE(MyParser);
            PARSER_CASE(GoogleParser);
            PARSER_CASE(NeonParser);
            PARSER_CASE(HtmlCxxParser);
            PARSER_CASE(RegExpParser);
        default:
            return "unknown";
        }
}

class TUrlParseResult {
public:
    TUrlParseResult()
    : m_sHost("")
    , m_sRequest("")
    {}

    TUrlParseResult(const std::string& host, const std::string& request)
    : m_sHost(host)
    , m_sRequest(request)
    {}

    TUrlParseResult( const TUrlParseResult& another )
    : m_sHost(another.m_sHost)
    , m_sRequest(another.m_sRequest)
    {}
    
    bool operator == ( const TUrlParseResult& another ) const
    {
        return ((m_sHost == another.m_sHost) and (m_sRequest == another.m_sRequest));
    }
    
    bool isNull() const
    {
        return ((m_sHost.empty()) and (m_sRequest.empty()));
    }

	std::string m_sHost;
	std::string m_sRequest;
};

typedef std::vector<TUrlParseResult> TParsersResults;

static TParsersResults createResults()
{
	return TParsersResults(eParsersAtAll, TUrlParseResult());
}

typedef std::vector<eUrlParsers>        TEquivalenceClass;
typedef std::vector<TEquivalenceClass>  TEquivalenceRelation;
typedef std::vector<bool>               TIncludedParser;

static TEquivalenceRelation FindRelated( const TParsersResults& results )
{
    TEquivalenceRelation    ret;    
    unsigned int            parsersCount(results.size());
    TIncludedParser         included(static_cast<unsigned int>(parsersCount), false);
    
    for (unsigned int i = 0; i < parsersCount; ++i) {
        
        if (included[i]) {
            continue;
        }
        
        const TUrlParseResult&  currentResult(results[i]);
        
        if (currentResult.isNull()) {
            continue;
        }
        
        TEquivalenceClass   currentClass(1, static_cast<eUrlParsers>(i));
        
        for (unsigned int j = i + 1; j < parsersCount; ++j) {
            
            if (included[j]) {
                continue;
            }
            
            const TUrlParseResult&  secondResult(results[j]);
            
            if (secondResult.isNull()) {
                continue;
            }
            
            if (secondResult == currentResult) {
                included[j] =   true;
                currentClass.push_back(static_cast<eUrlParsers>(j));
            }            
        }
        
        ret.push_back(currentClass);
    }
    
    return ret;
}

class TParserMark {
public:
    TParserMark()
    : m_iCorrectCount(0)
    , m_iTimeConsumed(0)
    {}

    unsigned int m_iCorrectCount;
    unsigned int m_iTimeConsumed;
};

typedef std::vector<TParserMark>    TParsersMark;

TParsersMark    createParsersMark()
{
    return TParsersMark(eParsersAtAll, TParserMark());
}

//<MyParser>
void myParseBase( CLinkFactory& factory, TEmptyAcceptor& acceptor, const std::string& baseUrl )
{
    factory.setAcceptor(acceptor);
    factory.pushLink(baseUrl);
    factory.setFrom(acceptor.m_tLink);
}

TUrlParseResult myParseRel( CLinkFactory& factory, TEmptyAcceptor& acceptor, const std::string& relativeUrl )
{
    factory.pushLink(relativeUrl);
    return TUrlParseResult(acceptor.m_tLink.getServer(), acceptor.m_tLink.getUri());
}


class TMyParser {
public:
    TMyParser()
    {
        m_tFactory.setAcceptor(m_tAcceptor);
    }

    void ParseBase( const std::string& baseUri )
    {
        m_tFactory.pushLink(baseUri);
        m_tFactory.setFrom(m_tAcceptor.m_tLink);
    }

    void ParseRel( const std::string& relUri )
    {
        m_tFactory.pushLink(relUri);
    }

private:
    TEmptyAcceptor  m_tAcceptor;
    CLinkFactory    m_tFactory;

};
//</MyParser>

//<google-url>
GURL  googleParseBase( const std::string& baseUrl )
{
    return GURL(baseUrl);
}

TUrlParseResult googleParseRel( GURL& baseUrl, const std::string& relativeUrl )
{
    GURL    relativeGurl(baseUrl.Resolve(relativeUrl));
    return TUrlParseResult(relativeGurl.host(), relativeGurl.PathForRequest());
}

class TGurlClass {
public:
    void ParseBase( const std::string& baseUri )
    {
        m_tUri  =   googleParseBase(baseUri);
    }

    void ParseRel( const std::string& relUri )
    {
        googleParseRel(m_tUri, relUri);
    }

private:
    GURL    m_tUri;
};
//</google-url>

//<htmlcxx>
htmlcxx::Uri htmlCxxParseBase( const std::string& baseUrl )
{
    return htmlcxx::Uri(htmlcxx::Uri::decode(baseUrl));
}

TUrlParseResult htmlCxxParseRel( htmlcxx::Uri& baseUri, const std::string& relativeUrl )
{
    htmlcxx::Uri    tmp(htmlcxx::Uri::decode(relativeUrl));
    htmlcxx::Uri    relativeUri(tmp.absolute(baseUri));

    return TUrlParseResult(
                            relativeUri.hostname(),
                            relativeUri.path() + ( relativeUri.existsQuery()
                                                        ? "?" + relativeUri.query()
                                                        : "" )
                           );
}

class THtmlCxxClass {
public:
    void ParseBase( const std::string& baseUri )
    {
        m_tUri  =   htmlCxxParseBase(baseUri);
    }

    void ParseRel( const std::string& relUri )
    {
        htmlCxxParseRel(m_tUri, relUri);
    }

private:
    htmlcxx::Uri    m_tUri;
};

//</htmlcxx>

template <class T>
void fullTest(const THtmlTaskList& tasks)
{
    T   parser;
    for (THtmlTaskList::const_iterator task = tasks.begin(); task != tasks.end(); ++task) {
        parser.ParseBase(task->m_sBaseUri);

        const THtmlTask::TUriList&  currentUris(task->m_lUris);
        for (
                THtmlTask::TUriList::const_iterator uri = currentUris.begin();
                uri != currentUris.end();
                ++uri
             ) {
           parser.ParseRel(*uri);
       }
    }
}

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << argv[0] << " \"link file\"" << std::endl;
        return 1;
    }

    std::ifstream   file(argv[1]);

    if (not file.is_open()) {
        std::cerr << "couldnt open '" << argv[1] << "' file" << std::endl;
        return 1;
    }

    THtmlTaskList   tasks;

    readTasksFromFile(file, tasks);
    
    unsigned int    currentTaskBlock(0);

    std::cout << "=============================" << std::endl;
    std::cout << "\tCorrectness testing" << std::endl;
    std::cout << "=============================" << std::endl;

    TParsersMark                    marks(createParsersMark());
    THtmlTaskList::const_iterator   task(tasks.begin());
    bool                            isStopExperiment(false);



    while ((task != tasks.end()) and (not isStopExperiment)) {
        const THtmlTask::TUriList&  currentUris(task->m_lUris);
        const std::string&          strBaseUrl(task->m_sBaseUri);

        GURL                        gUrl(googleParseBase(strBaseUrl));

        CLinkFactory                linkFactory;
        TEmptyAcceptor              acceptor;
                
        myParseBase(linkFactory, acceptor, strBaseUrl);

        htmlcxx::Uri                hUrl(htmlCxxParseBase(strBaseUrl));
        
        unsigned int                        currentTask(0);
        THtmlTask::TUriList::const_iterator uri(currentUris.begin());

        while (( uri != currentUris.end()) and (not isStopExperiment)) {
            
            TParsersResults results(createResults());

            //duplicate values for testing purposes
            results[eGoogleParser]  =   googleParseRel(gUrl, *uri);
            results[eHtmlCxxParser] =   htmlCxxParseRel(hUrl, *uri);
            results[eMyParser]      =   myParseRel(linkFactory, acceptor, *uri);
            
            TEquivalenceRelation    equivalenceClasses(FindRelated(results));            
            unsigned int            classesCount(equivalenceClasses.size());            
            if (classesCount > 1) {
                std::cout   << "=== block # " << currentTaskBlock
                            << " task # " << currentTask << " ===" << std::endl;
                
                for (unsigned int clas = 0; clas != classesCount; ++clas) {
                    
                    const TEquivalenceClass&    currentClass(equivalenceClasses[clas]);
                    
                    for (unsigned int parser = 0; parser != currentClass.size(); ++parser) {
                        std::cout   << getParserName(currentClass[parser])
                                    << " : " << std::endl;
                    }
                    const TUrlParseResult&  resultOfClass(results[*currentClass.begin()]);
                    
                    std::cout << "\thost    : " << resultOfClass.m_sHost << std::endl;
                    std::cout << "\trequest : " << resultOfClass.m_sRequest << std::endl;
                }


                std::cout << "Who is rigth?" << std::endl;
                
                for (unsigned int clas = 0; clas != classesCount; ++clas) {
                    const TEquivalenceClass&    currentClass(equivalenceClasses[clas]);

                    std::cout << clas << " ) ";
                    for (unsigned int parser = 0; parser != currentClass.size(); ++parser) {
                        std::cout   << getParserName(currentClass[parser])
                                    << " & ";
                    }
                    std::cout << std::endl;
                }
                std::cout << classesCount << " ) Nobody" << std::endl;
                std::cout << classesCount + 1 << " ) Stop Experiment" << std::endl;

                unsigned int choice(classesCount + 1);
                std::cout   << "Enter number : ";
                std::cin    >> choice;
                std::cout   << std::endl;

                if (choice < classesCount) {
                    const TEquivalenceClass&    correctClass(equivalenceClasses[choice]);
                    for (unsigned int parser = 0; parser != correctClass.size(); ++parser) {                        
                        ++marks[static_cast<unsigned int>(correctClass[parser])].m_iCorrectCount;
                    }
                    //TODO : add correct answer recording
                }
                else if ((classesCount + 1) == choice) {
                    isStopExperiment    =   true;
                }
            }
            else {
                //everybody cant be wrong
                for (unsigned int i = 0; i != marks.size(); ++i) {
                    ++marks[i].m_iCorrectCount;
                }
                //TODO : add correct answer recording
            }
            
            ++currentTask;
            ++uri;
        }
        ++currentTaskBlock;
        ++task;
    }

    std::cout << "=============================" << std::endl;
    std::cout << "\tSpeed testing" << std::endl;
    std::cout << "=============================" << std::endl;

    ptime    start(microsec_clock::local_time());
    fullTest<TGurlClass>(tasks);
    ptime    stop(microsec_clock::local_time());

    marks[static_cast<unsigned int>(eGoogleParser)].m_iTimeConsumed =   time_period(start, stop).length().total_microseconds();

    start   =   microsec_clock::local_time();
    fullTest<THtmlCxxClass>(tasks);
    stop    =   microsec_clock::local_time();


    marks[static_cast<unsigned int>(eHtmlCxxParser)].m_iTimeConsumed =   time_period(start, stop).length().total_microseconds();


    start   =   microsec_clock::local_time();
    fullTest<TMyParser>(tasks);
    stop    =   microsec_clock::local_time();

    marks[static_cast<unsigned int>(eMyParser)].m_iTimeConsumed =   time_period(start, stop).length().total_microseconds();

    std::cout << "=============================" << std::endl;
    std::cout << "\tResults" << std::endl;
    std::cout << "=============================" << std::endl;

    for (unsigned int i = 0; i < marks.size(); ++i) {
        std::cout   << getParserName(static_cast<eUrlParsers>(i))
                    << " : " << std::endl;
        std::cout   << "\tcorrect : " << marks[i].m_iCorrectCount << std::endl;
        std::cout   << "\ttime    : " << marks[i].m_iTimeConsumed << std::endl;
    }




    return 0;
}

