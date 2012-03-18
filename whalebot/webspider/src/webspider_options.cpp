#include <iostream>
#include <fstream>


#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <webspider_options.h>
#include <version.h>
#include <config_const.h>

using namespace boost::program_options;


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

, m_sOptionsFile("")

{}


bool CWebSpiderOptions::readFromCmdLine(int argc, char* argv[])
{

    options_description desc("Webspider options");
    desc.add_options()
            ("help,h",    "show this message")
            ("version,v", "show version info")

            ("site,s", value<std::string>(&m_sSite)->default_value(""), "seed uri")

            (kSavePagesAttrCmd.c_str(), "dont save fetched pages")
            (kOneServerAttrCmd.c_str(), "fetch only from one server")

            ( kLinkFilterFileAttrCmd.c_str()
            , value<std::string>(&m_sLinkFilterFile)->default_value("")
            , "file with words to link filter" )

            ( kLevelAttrCmd.c_str()
            , value<size_t>(&m_iLevel)->default_value(0)
            , "max level of link in one site" )

            ( kOutputAttrCmd.c_str()
            , value<std::string>(&m_sOutput)->default_value(boost::filesystem::initial_path().native())
            , "output dir" )

            ( kErrorLogPathAttrCmd.c_str()
            , value<std::string> (&m_sErrorLogPath)->default_value("")
            , "path to log file" )

            ( kTmpFilePathAttrCmd.c_str()
            , value<std::string> (&m_sTmpFilePath)->default_value("tmp")
            , "tmp filename" )

            ( kUsedLinksPathAttrCmd.c_str()
            , value<std::string>(&m_sUsedLinksPath)->default_value("usedlinks.txt")
            , "path to used links file" )

            ( kFutureLinksPathAttrCmd.c_str()
            , value<std::string> (&m_sFutureLinksPath)->default_value("futurelinks.txt")
            , "path future links file" )

            ( kConnectionTimeoutInSecondsAttrCmd.c_str()
            , value<unsigned int> (&m_iConnectionTimeoutInSeconds)->default_value(5)
            , "connection timeout" )

            ( kReadTimeoutInSecondsAttrCmd.c_str()
            , value<unsigned int> (&m_iReadTimeoutInSeconds)->default_value(2)
            , "read timeout")

            ( kMaxConnectionsAttrCmd.c_str()
            , value<unsigned int> (&m_iMaxConnections)->default_value(20)
            , "qantity of simulteniously open connections (unused at this moment)" )


            (kCollectLinksAttrCmd.c_str(),  "collect links")
            (kSaveHistoryAttrCmd.c_str(),   "do not save links after stop")
            (kAskAfterFetchAttrCmd.c_str(), "ask after fetching")

            ( kOptionsFileAttrCmd.c_str()
            , value<std::string>(&m_sOptionsFile)->default_value("")
            , "file with configurations ");

    positional_options_description p;
    p.add("site", -1);

    variables_map vm;

    try {
        store( command_line_parser(argc, argv).options(desc).positional(p).run()
             , vm);
    }
    catch (boost::program_options::error& /*e*/) {
        std::cerr << "wrong cmd line parametrs" << std::endl;
        return false;
    }

    notify(vm);

    if (vm.count("version")) {
        std::cout   << "WebSpider - tool for web grabbing v "
                    << kVersion
                    << " - Beta" << std::endl
                    << "Author whalebot.helmsman@gmail.com started 15.03.2009" << std::endl;
        return false;
    }

    if (vm.count("help")) {
        std::cout << "Usage: options_description [options]\n";
        std::cout << desc;
        return false;
    }

    if (vm.count(kAskAfterFetchAttr)) {
        m_bAskAfterFetch =   true;
    }

    if (vm.count(kSavePagesAttr)) {
        m_bSavePages  =   false;
    }


    if (vm.count(kOneServerAttr)) {
        m_bOneServer  =   true;
    }

    if(vm.count(kSaveHistoryAttr)){
        m_bSaveHistory    =   false;
    }

    if(vm.count(kCollectLinksAttr)){
        m_bCollectLinks   =   true;
    }

    if(not m_sOptionsFile.empty()) {
        readFromFile(m_sOptionsFile);
    }

    return true;
}


bool CWebSpiderOptions::readFromFile(const std::string& path)
{
    std::ifstream   file(path.c_str());

    if (not file.is_open()) {
        return false;
    }

//    COptions    options;
//    options.read(file);


//    options.get(kAskAfterFetchAttr,              m_bAskAfterFetch);
//    options.get(kCollectLinksAttr,               m_bCollectLinks);
//    options.get(kConnectionTimeoutInSecondsAttr, m_iConnectionTimeoutInSeconds);
//    options.get(kErrorLogPathAttr,               m_sErrorLogPath);
//    options.get(kFutureLinksPathAttr,            m_sFutureLinksPath);
//    options.get(kLevelAttr,                      m_iLevel);
//    options.get(kLinkFilterFileAttr,             m_sLinkFilterFile);
//    options.get(kMaxConnectionsAttr,             m_iMaxConnections);
//    options.get(kOneServerAttr,                  m_bOneServer);
//    options.get(kOutputAttr,                     m_sOutput);
//    options.get(kReadTimeoutInSecondsAttr,       m_iReadTimeoutInSeconds);
//    options.get(kSaveHistoryAttr,                m_bSaveHistory);
//    options.get(kSavePagesAttr,                  m_bSavePages);
//    options.get(kTmpFilePathAttr,                m_sTmpFilePath);
//    options.get(kUsedLinksPathAttr,              m_sUsedLinksPath);

    return true;
}
