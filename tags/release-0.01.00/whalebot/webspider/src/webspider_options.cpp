#include <iostream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "webspider_options.h"
#include "version.h"




CWebSpiderOptions::CWebSpiderOptions()
: m_sSite("")
, m_sLinkFilterFile("")
, m_sErrorLogPath("")
, m_sOutput("")
, m_sTmpFilePath("")
, m_sUsedLinksPath("")
, m_sFutureLinksPath("")

, m_bSavePages(true)
, m_bSaveHistory(true)
, m_bCollectLinks(false)
, m_bOneServer(false)
, m_bAskAfterFetch(false)

, m_iLevel(0)
, m_iConnectionTimeoutInSeconds(0)
, m_iReadTimeoutInSeconds(0)
, m_iMaxConnections(0)

{}


bool CWebSpiderOptions::readFromCmdLine(int argc, char* argv[])
{

    boost::program_options::options_description desc("Webspider options");
    desc.add_options()
            ("help,h", "show this message")
            ("version,v", "show version info")

            ("site,s", boost::program_options::value<std::string> (&m_sSite)->default_value(""), "seed uri")

            ("dont-save,d", "dont save fetched pages")

            ("one-server,o", "fetch only from one server")
            ("link-filter,l", boost::program_options::value<std::string> (&m_sLinkFilterFile)->default_value(""), "file with words to link filter")
            ("max-level,m", boost::program_options::value<size_t> (&m_iLevel)->default_value(0), "max level of link in one site")

            ("output,O", boost::program_options::value<std::string> (&m_sOutput)->default_value(boost::filesystem::initial_path().native_directory_string()), "output dir")
            ("errorlog,e", boost::program_options::value<std::string> (&m_sErrorLogPath)->default_value(""), "path to log file")
            ("tmp-file,t", boost::program_options::value<std::string> (&m_sTmpFilePath)->default_value("tmp"), "tmp filename")

            ("usedlinks,u", boost::program_options::value<std::string> (&m_sUsedLinksPath)->default_value("usedlinks.txt"), "path to used links file")
            ("futurelinks,f", boost::program_options::value<std::string> (&m_sFutureLinksPath)->default_value("futurelinks.txt"), "path future links file")

            ("connection-timeout,T", boost::program_options::value<unsigned int> (&m_iConnectionTimeoutInSeconds)->default_value(5), "connection timeout")
            ("read-timeout,R", boost::program_options::value<unsigned int> (&m_iReadTimeoutInSeconds)->default_value(2), "read timeout")
            ("max-connections,M", boost::program_options::value<unsigned int> (&m_iMaxConnections)->default_value(20), "qantity of simulteniously open connections")


            ("collect-links,c" , "collect links")
            ("dont-save-links,D",  "do not save links after stop")
            ("ask,a",  "ask after fetching")
            ;

    boost::program_options::positional_options_description p;
    p.add("site", -1);

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    boost::program_options::notify(vm);

    if (vm.count("version")) {
        std::cout   << "WebSpider - tool for statsem package v " << kVersion << " - Beta" << std::endl
                    << "Author Vostretsov Nikita started 15.03.2009" << std::endl;
        return false;
    }

    if (vm.count("help")) {
        std::cout << "Usage: options_description [options]\n";
        std::cout << desc;
        return false;
    }

    if (vm.count("ask")) {
        m_bAskAfterFetch =   true;
    }

    if (vm.count("dont-save")) {
        m_bSavePages  =   false;
    }


    if (vm.count("one-server")) {
        m_bOneServer  =   true;
    }

    if(vm.count("dont-save-links")){
        m_bSaveHistory    =   false;
    }

    if(vm.count("collect-links")){
        m_bCollectLinks   =   true;
    }

    return true;

}
