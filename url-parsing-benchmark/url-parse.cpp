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

#include <to_string.h>


#include "html_task.h"
#include "parser_type.h"
#include "answer.h"
#include "equivalence.h"
#include "answer_base.h"
#include "htmlcxx_parser.h"
#include "whalebot_parser.h"
#include "gurl_parser.h"
#include "neon_parser.h"

#include "group_errorwriter.h"


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

template <class T>
static void fullTest( const THtmlTaskList& tasks )
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
static unsigned int speedTest( const THtmlTaskList& tasks )
{
    ptime   start(microsec_clock::local_time());
    fullTest<T>(tasks);
    ptime   stop(microsec_clock::local_time());


    return time_period(start, stop).length().total_microseconds();
}



static void showHeader( std::ostream& stream, const std::string& label )
{
    stream << "============================" << std::endl;
    stream << '\t' << label << std::endl;
    stream << "============================" << std::endl;
}



int main( int argc, char** argv ) {

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
        }
    }

    TGroupErrorWriter   errors;

    if (not errors.init()) {
        std::cout   << "Not all error writers loaded correctly. Continue ? [y/n]"
                    << std::endl;

        char response(0);

        std::cin >> response;

        if ((response == 'n') or (response == 'N')) {
            std::cout << "Program stopped" << std::endl;
            return 1;
        }
    }
    
    unsigned int    currentTaskBlock(0);


    showHeader(std::cout, "Correctness testing");


    TParsersMark                    marks(createParsersMark());
    THtmlTaskList::const_iterator   task(tasks.begin());
    bool                            isStopExperiment(false);



    while ((task != tasks.end()) and (not isStopExperiment)) {
        const THtmlTask::TUriList&  currentUris(task->m_lUris);
        const std::string&          strBaseUrl(task->m_sBaseUri);

        GURL                        gUrl(gurl::gParseBase(strBaseUrl));

        CLinkFactory                linkFactory;
        whalebot::CEmptyAcceptor    acceptor;
        whalebot::gParseBase(linkFactory, acceptor, strBaseUrl);

        htmlcxx::Uri                hUrl(htmlcxx::gParseBase(strBaseUrl));

        ne_uri                      nUrl(neon::gParseBase(strBaseUrl));
        
        unsigned int                currentTask(0);
        THtmlTask::TUriList::const_iterator uri(currentUris.begin());

        while (( uri != currentUris.end()) and (not isStopExperiment)) {
            
            TParsersResults results(gCreateResults());

            //duplicate values for testing purposes
            results[eGoogleParser]  =   gurl::gParseRel(gUrl, *uri);
            results[eHtmlCxxParser] =   htmlcxx::gParseRel(hUrl, *uri);
            results[eMyParser]      =   whalebot::gParseRel(linkFactory, acceptor, *uri);
            results[eNeonParser]    =   neon::gParseRel(&nUrl, *uri);

            THtmlAnswer     correctAnswer;


            if (correctAnswer.isNull()) {
                const THtmlAnswer* const
                        answerFromDb(rigthAnswers.getAnswer(currentTaskBlock,
                                                            currentTask)
                                    );

                if ((0 != answerFromDb) and (not answerFromDb->isNull())) {
                    correctAnswer   =   *answerFromDb;
                }
            }

            TEquivalenceRelation    equivalenceClasses(gFindRelated(results));
            unsigned int            classesCount(equivalenceClasses.size());

            if (correctAnswer.isNull()) {
                if (classesCount == 1) {
                    correctAnswer   =   results[*(equivalenceClasses.begin()->begin())];
                }
            }

            if (correctAnswer.isNull()) {

                std::string blockHeader("block # ");
                blockHeader +=  toString(currentTaskBlock);
                blockHeader +=  " task # ";
                blockHeader +=  toString(currentTask);

                showHeader(std::cout, blockHeader);

                std::cout << "Task is   : " << std::endl;

                std::cout << "\tbase url     : " << strBaseUrl << std::endl;
                std::cout << "\trelative url : " << *uri << std::endl;

                std::cout << "Answer is : " << std::endl;


                for (unsigned int clas = 0; clas != classesCount; ++clas) {

                    const TEquivalenceClass&    currentClass(equivalenceClasses[clas]);

                    for (unsigned int parser = 0; parser != currentClass.size(); ++parser) {
                        std::cout   << '\t' << gGetParserName(currentClass[parser])
                                    << " : " << std::endl;
                    }
                    const TUrlParseResult&  resultOfClass(results[*currentClass.begin()]);

                    std::cout << "\t\thost    : " << resultOfClass.m_sHost << std::endl;
                    std::cout << "\t\trequest : " << resultOfClass.m_sRequest << std::endl;
                }


                std::cout << "Who is rigth?" << std::endl;

                for (unsigned int clas = 0; clas != classesCount; ++clas) {
                    const TEquivalenceClass&    currentClass(equivalenceClasses[clas]);

                    std::cout << clas + 1 << " ) ";
                    for (unsigned int parser = 0; parser != currentClass.size(); ++parser) {
                        std::cout   << gGetParserName(currentClass[parser])
                                    << " & ";
                    }
                    std::cout << std::endl;
                }
                std::cout << classesCount + 1 << " ) Nobody" << std::endl;
                std::cout << classesCount + 2 << " ) Stop Experiment" << std::endl;

                unsigned int choice(classesCount + 2);
                std::cout   << "Enter number : ";
                std::cin    >> choice;
                std::cout   << std::endl;

                if (choice - 1 < classesCount) {
                    const TEquivalenceClass&    correctClass(equivalenceClasses[choice - 1]);
                    correctAnswer   =   results[static_cast<unsigned int>(*correctClass.begin())];
                }
                else if ((classesCount + 1) == (choice - 1)) {
                    isStopExperiment    =   true;
                }
            }

            if (not correctAnswer.isNull()) {

                for (unsigned int i = 0; i != results.size(); ++i) {
                    if (correctAnswer == results[i]) {
                        ++(marks[i].m_iCorrectCount);
                    }
                    else {
                        errors.writeError( static_cast<eUrlParsers>(i)
                                         , strBaseUrl
                                         , *uri );
                    }
                }

                rigthAnswers.addAnswer( currentTaskBlock
                                      , currentTask
                                      , correctAnswer);
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


    showHeader(std::cout, "Speed testing");

    marks[static_cast<unsigned int>(eGoogleParser)].m_iTimeConsumed     =   speedTest<gurl::CParser>(tasks);
    marks[static_cast<unsigned int>(eHtmlCxxParser)].m_iTimeConsumed    =   speedTest<htmlcxx::CParser>(tasks);
    marks[static_cast<unsigned int>(eMyParser)].m_iTimeConsumed         =   speedTest<whalebot::CParser>(tasks);
    marks[static_cast<unsigned int>(eNeonParser)].m_iTimeConsumed       =   speedTest<neon::CParser>(tasks);

    showHeader(std::cout, "Results");

    for (unsigned int i = 0; i < marks.size(); ++i) {
        std::cout   << gGetParserName(static_cast<eUrlParsers>(i))
                    << " : " << std::endl;
        std::cout   << "\tcorrect : " << marks[i].m_iCorrectCount << std::endl;
        std::cout   << "\ttime    : " << marks[i].m_iTimeConsumed << std::endl;
    }

    return 0;
}

