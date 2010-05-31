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

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include <googleurl/src/gurl.h>
#include <htmlcxx/html/Uri.h>
#include <htmlcxx/html/utils.h>

#include <neon/ne_uri.h>

#include "../whalebot/webspider/include/link_factory.h"
#include "../whalebot/webspider/include/prefix.h"

using namespace boost::posix_time;

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

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive& ar, TUrlParseResult& res, const unsigned int version)
{

    ar & res.m_sHost;
    ar & res.m_sRequest;
}

} // namespace serialization
} // namespace boost


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

class TEmptyAcceptor : public IAcceptor {
public:
    void    pushLink( const CLink& link )
    {
        m_tLink =   link;
        return;
    }

    CLink   m_tLink;

};

void myParseBase( CLinkFactory& factory,
                  TEmptyAcceptor& acceptor,
                  const std::string& baseUrl )
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


class TMyClass {
public:
    TMyClass()
    {
        m_tFactory.setAcceptor(m_tAcceptor);
    }

    void parseBase( const std::string& baseUri )
    {
        m_tFactory.pushLink(baseUri);
        m_tFactory.setFrom(m_tAcceptor.m_tLink);
    }

    void parseRel( const std::string& relUri )
    {
        m_tFactory.pushLink(relUri);
    }

private:
    TEmptyAcceptor  m_tAcceptor;
    CLinkFactory    m_tFactory;

};
//</MyParser>

//<libneon>
ne_uri neonParseBase( const std::string& baseUrl )
{
    ne_uri base;
    char*   decoded(ne_path_unescape(baseUrl.c_str()));
    ne_uri_parse(decoded, &base);
    free(decoded);

    return base;
}

TUrlParseResult neonParseRel( ne_uri* baseUrl, const std::string& relativeUrl )
{
    ne_uri  relUrl;
    char*   decoded(ne_path_unescape(relativeUrl.c_str()));
    ne_uri_parse(decoded, &relUrl);
    free(decoded);

    ne_uri  resUrl;
    ne_uri_resolve(baseUrl, &relUrl, &resUrl);

    TUrlParseResult ret(resUrl.host, resUrl.path);

    if (0 != resUrl.query) {
        ret.m_sRequest.append(1, '?');
        ret.m_sRequest.append(resUrl.query);
    }

    ne_uri_free(&resUrl);
    ne_uri_free(&relUrl);

    return ret;
}

class TNeonClass {
public:

    TNeonClass()
    {
        ne_uri_parse("www.example.com", &m_tUri);
    }

    void parseBase( const std::string& baseUri )
    {
        ne_uri_free(&m_tUri);
        m_tUri  =   neonParseBase(baseUri);
    }

    void parseRel( const std::string& relUri )
    {
        neonParseRel(&m_tUri, relUri);
    }

    ~TNeonClass()
    {
        ne_uri_free(&m_tUri);
    }

private:
    ne_uri m_tUri;
};

//<\libneon>

//<google-url>
GURL  googleParseBase( const std::string& baseUrl )
{
    return GURL(htmlcxx::Uri::decode(baseUrl));
}

TUrlParseResult googleParseRel( GURL& baseUrl, const std::string& relativeUrl )
{
    GURL   relativeGurl(baseUrl.Resolve(htmlcxx::Uri::decode(relativeUrl)));
    return TUrlParseResult(relativeGurl.host(), relativeGurl.PathForRequest());
}

class TGurlClass {
public:
    void parseBase( const std::string& baseUri )
    {
        m_tUri  =   googleParseBase(baseUri);
    }

    void parseRel( const std::string& relUri )
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
    void parseBase( const std::string& baseUri )
    {
        m_tUri  =   htmlCxxParseBase(baseUri);
    }

    void parseRel( const std::string& relUri )
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
    T               parser;
    unsigned int    blockNum(0);
    for (THtmlTaskList::const_iterator task = tasks.begin(); task != tasks.end(); ++task) {
        parser.parseBase(task->m_sBaseUri);

        const THtmlTask::TUriList&  currentUris(task->m_lUris);
        for (
                THtmlTask::TUriList::const_iterator uri = currentUris.begin();
                uri != currentUris.end();
                ++uri
             ) {
           parser.parseRel(*uri);
       }

       ++blockNum;
    }
}


typedef TUrlParseResult             THtmlAnswer;
typedef std::vector<THtmlAnswer>    TAnswersList;
typedef std::vector<TAnswersList>   TAnswersBase;


class CAnswerBase {
public:

    void addAnswer( unsigned int taskBlockNum,
                    unsigned int taskNum,
                    const THtmlAnswer& answer );

    const THtmlAnswer* const getAnswer( unsigned int taskBlockNum,
                                        unsigned int taskNum ) const;

    bool read( const std::string& path );

    bool write( const std::string& path );

    TAnswersBase m_tBase;
};

void CAnswerBase::addAnswer( unsigned int taskBlockNum,
                             unsigned int taskNum,
                             const THtmlAnswer& answer )
{
    if (m_tBase.size() <= taskBlockNum) {
        m_tBase.resize(taskBlockNum + 1);
    }

    TAnswersList&   curList(m_tBase[taskBlockNum]);

    if (curList.size() <= taskNum) {
        curList.resize(taskNum + 1);
    }

    curList[taskNum]    =   answer;

}

const THtmlAnswer* const CAnswerBase::getAnswer(    unsigned int taskBlockNum,
                                                    unsigned int taskNum ) const {

    if (m_tBase.size() <= taskBlockNum) {
        return 0;
    }

    const TAnswersList&   curList(m_tBase[taskBlockNum]);

    if (curList.size() <= taskNum) {
        return 0;
    }

    return &curList[taskNum];
}



namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive& ar, CAnswerBase& base, const unsigned int version)
{
    serialize(ar, base.m_tBase, version);
}

} // namespace serialization
} // namespace boost

bool CAnswerBase::read( const std::string &path )
{
    std::ifstream                   ifs(path.c_str());

    if (not ifs.is_open()) {
        return false;
    }

    boost::archive::text_iarchive   ia(ifs);
    boost::serialization::serialize(ia, *this, 1);

    return true;
}

bool CAnswerBase::write( const std::string &path )
{
    std::ofstream                   ofs(path.c_str());

    if (not ofs.is_open()) {
        return false;
    }
    boost::archive::text_oarchive   oa(ofs);
    boost::serialization::serialize(oa, *this, 1);

    return true;
}


int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << argv[0] << " \"link file\" [\"answer base\"]" << std::endl;
        return 1;
    }



    std::ifstream   file(argv[1]);

    if (not file.is_open()) {
        std::cerr << "couldnt open '" << argv[1] << "' file" << std::endl;
        return 1;
    }
    THtmlTaskList   tasks;
    readTasksFromFile(file, tasks);


    std::string answerBasePath("");

    if (argc >= 3) {
        answerBasePath  =   argv[2];
    }

    CAnswerBase     rigthAnswers;

    if (not answerBasePath.empty()) {
        if (not rigthAnswers.read(answerBasePath)) {
            std::cerr << "couldnt load answers from " << answerBasePath << std::endl;
            //return 1;
        }
    }
    
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

        ne_uri                      nUrl(neonParseBase(strBaseUrl));
        
        unsigned int                currentTask(0);
        THtmlTask::TUriList::const_iterator uri(currentUris.begin());

        while (( uri != currentUris.end()) and (not isStopExperiment)) {
            
            TParsersResults results(createResults());

            //duplicate values for testing purposes
            results[eGoogleParser]  =   googleParseRel(gUrl, *uri);
            results[eHtmlCxxParser] =   htmlCxxParseRel(hUrl, *uri);
            results[eMyParser]      =   myParseRel(linkFactory, acceptor, *uri);
            results[eNeonParser]    =   neonParseRel(&nUrl, *uri);


            const THtmlAnswer* const curAnswer(
                                                rigthAnswers.getAnswer(
                                                                        currentTaskBlock,
                                                                        currentTask
                                                                      )
                                              );

            if ((0 == curAnswer) or (curAnswer->isNull())) {

                //we dont know correct answer

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
                        rigthAnswers.addAnswer(
                                                currentTaskBlock,
                                                currentTask,
                                                results[*correctClass.begin()]
                                              );
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
                    rigthAnswers.addAnswer(
                                            currentTaskBlock,
                                            currentTask,
                                            results[*(equivalenceClasses.begin()->begin())]
                                          );
                }

            }
            else {
                //we know correct answer
                for (unsigned int i = 0; i < results.size(); ++i) {
                    const TUrlParseResult&  result(results[i]);
                    if (result == *curAnswer) {
                        marks[i].m_iCorrectCount += 1;
                    }
                }
            }
            

            
            ++currentTask;
            ++uri;
        }
        ne_uri_free(&nUrl);

        ++currentTaskBlock;
        ++task;
    }

    if (not answerBasePath.empty()) {
        if (not rigthAnswers.write(answerBasePath)) {
            std::cerr << "couldnt write answers to " << answerBasePath << std::endl;
        }
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
    fullTest<TMyClass>(tasks);
    stop    =   microsec_clock::local_time();

    marks[static_cast<unsigned int>(eMyParser)].m_iTimeConsumed =   time_period(start, stop).length().total_microseconds();


    start   =   microsec_clock::local_time();
    fullTest<TNeonClass>(tasks);
    stop    =   microsec_clock::local_time();

    marks[static_cast<unsigned int>(eNeonParser)].m_iTimeConsumed =   time_period(start, stop).length().total_microseconds();

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

