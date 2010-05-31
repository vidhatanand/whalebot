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


#include <neon/ne_uri.h>

#include "html_task.h"
#include "parser_type.h"
#include "answer.h"
#include "equivalence.h"
#include "answer_base.h"
#include "htmlcxx_parser.h"
#include "whalebot_parser.h"


using namespace boost::posix_time;


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

static TParsersMark createParsersMark()
{
    return TParsersMark(eParsersAtAll, TParserMark());
}



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



template <class T>
static void fullTest(const THtmlTaskList& tasks)
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

template <class T>
static unsigned int speedTest(const THtmlTaskList& tasks)
{
    ptime   start(microsec_clock::local_time());
    fullTest<T>(tasks);
    ptime   stop(microsec_clock::local_time());


    return time_period(start, stop).length().total_microseconds();
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
    gReadTasksFromFile(file, tasks);


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
        whalebot::CEmptyAcceptor    acceptor;
        whalebot::gParseBase(linkFactory, acceptor, strBaseUrl);

        htmlcxx::Uri                hUrl(htmlcxx::gParseBase(strBaseUrl));

        ne_uri                      nUrl(neonParseBase(strBaseUrl));
        
        unsigned int                currentTask(0);
        THtmlTask::TUriList::const_iterator uri(currentUris.begin());

        while (( uri != currentUris.end()) and (not isStopExperiment)) {
            
            TParsersResults results(gCreateResults());

            //duplicate values for testing purposes
            results[eGoogleParser]  =   googleParseRel(gUrl, *uri);
            results[eHtmlCxxParser] =   htmlcxx::gParseRel(hUrl, *uri);
            results[eMyParser]      =   whalebot::gParseRel(linkFactory, acceptor, *uri);
            results[eNeonParser]    =   neonParseRel(&nUrl, *uri);


            const THtmlAnswer* const curAnswer(
                                                rigthAnswers.getAnswer(
                                                                        currentTaskBlock,
                                                                        currentTask
                                                                      )
                                              );

            if ((0 == curAnswer) or (curAnswer->isNull())) {

                //we dont know correct answer

                TEquivalenceRelation    equivalenceClasses(gFindRelated(results));
                unsigned int            classesCount(equivalenceClasses.size());
                if (classesCount > 1) {
                    std::cout   << "=== block # " << currentTaskBlock
                                << " task # " << currentTask << " ===" << std::endl;

                    for (unsigned int clas = 0; clas != classesCount; ++clas) {

                        const TEquivalenceClass&    currentClass(equivalenceClasses[clas]);

                        for (unsigned int parser = 0; parser != currentClass.size(); ++parser) {
                            std::cout   << gGetParserName(currentClass[parser])
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
                            std::cout   << gGetParserName(currentClass[parser])
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

    marks[static_cast<unsigned int>(eGoogleParser)].m_iTimeConsumed     =   speedTest<TGurlClass>(tasks);
    marks[static_cast<unsigned int>(eHtmlCxxParser)].m_iTimeConsumed    =   speedTest<htmlcxx::CParser>(tasks);
    marks[static_cast<unsigned int>(eMyParser)].m_iTimeConsumed         =   speedTest<whalebot::CParser>(tasks);
    marks[static_cast<unsigned int>(eNeonParser)].m_iTimeConsumed       =   speedTest<TNeonClass>(tasks);

    std::cout << "=============================" << std::endl;
    std::cout << "\tResults" << std::endl;
    std::cout << "=============================" << std::endl;

    for (unsigned int i = 0; i < marks.size(); ++i) {
        std::cout   << gGetParserName(static_cast<eUrlParsers>(i))
                    << " : " << std::endl;
        std::cout   << "\tcorrect : " << marks[i].m_iCorrectCount << std::endl;
        std::cout   << "\ttime    : " << marks[i].m_iTimeConsumed << std::endl;
    }

    return 0;
}

