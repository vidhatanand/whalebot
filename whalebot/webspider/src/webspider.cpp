#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/time_formatters.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/date_time/time_facet.hpp>


#include <neon/ne_session.h>

#include <one_fetcher.h>
#include <link_buffer.h>
#include <link_extractor.h>
#include <header_parser.h>
#include <filename_handler.h>
#include <filters.h>
#include <link.h>
#include <ilink_factory.h>
#include <link_factory.h>
#include <collector_link.h>
#include <webspider_options.h>

void async_read(bool &stop){
    getchar();
    stop   =   true;
}

int main(int argc, char* argv[]) {
    
    ne_sock_init();


    CWebSpiderOptions   options;    

    if (not options.readFromCmdLine(argc, argv)) {
        return 0;
    }
    
    bool                isTimeToStop(false);


    std::ostream    *errorLog(&std::cout);
    bool            isNeedDelete(false);
    if (!options.m_sErrorLogPath.empty()) {
        std::ofstream *file = new std::ofstream(options.m_sErrorLogPath.c_str());
        if (file->is_open()){
            errorLog        =   file;
            isNeedDelete    =   true;
        }
    }
    
    errorLog->imbue(std::locale(errorLog->getloc(), new boost::posix_time::time_facet("%T")));

    CFilenameHandler    files(options.m_sOutput);
    COneFetcher         fetcher;
    
    ILinkFactory*       factory(0);
    if (options.m_bCollectLinks) {
        factory =   new CCollectorLinkFactory();
    }
    else {
        factory =   new CLinkFactory();
    }

    CFilterList     filters;
    CLinkBuffer     work_front;
    factory->setAcceptor(filters);
    filters.setAcceptor(work_front);

    if(options.m_bOneServer){
        filters.addFilter(new COneServerFilter());
    }

    if(!options.m_sLinkFilterFile.empty()){
        std::ifstream   link_file(options.m_sLinkFilterFile.c_str());
        filters.addFilter(new CIncludeFilter(link_file));
    }

    if(options.m_iLevel > 0){
        filters.addFilter(new CLevelFilter(options.m_iLevel));
    }
    
    
    std::ifstream   usedlinks_file(options.m_sUsedLinksPath.c_str()),
                    futurelinks_file(options.m_sFutureLinksPath.c_str());

    if(futurelinks_file.is_open()){
        work_front.readFutureLinks(futurelinks_file);
        futurelinks_file.close();
        futurelinks_file.clear();
    }

    if(usedlinks_file.is_open()){
        work_front.readUsedLinks(usedlinks_file);
        usedlinks_file.close();
        usedlinks_file.clear();
    }
    
    factory->pushLink(options.m_sSite);

    CLink   next;
    bool    connected(false);
    int     link_counter(0),            
            http_errors(0);


    if (!options.m_bAskAfterFetch) {
        boost::thread   t(boost::bind(async_read, boost::ref(isTimeToStop)));
        std::cout<<"*Start working press [ENTER] to stop"<<std::endl;
    }

    boost::posix_time::ptime    start   =   boost::posix_time::microsec_clock::local_time();

    while ((work_front.pop(next))&&(!isTimeToStop)) {
        

        if(!next.isValid())
            continue;

        ++link_counter;

        boost::posix_time::ptime   now   =   boost::posix_time::microsec_clock::local_time();        
        

        (*errorLog) << now
                    << " we have " << work_front.size() + 1
                    << " links, looks at " << link_counter - 1
                    << " links, found "<< std::endl;

        double  time_consumption(boost::posix_time::time_period(start, now).length().total_microseconds());
        time_consumption    /=  1000000;
        (*errorLog) << "speed " << (link_counter - 1) / time_consumption << " links/sec" <<std::endl;
        (*errorLog) << "we have "<< http_errors << " errors" <<std::endl;

        if(options.m_bAskAfterFetch){
            char    c;
            std::cout<<"continue?(y/n)"<<std::endl;
            std::cin>>c;
            if(c == 'n'){
                isTimeToStop    =   true;
                continue;
            }
        }

        (*errorLog) << "*Connect to " << next.getServer() << std::endl;
        connected = fetcher.connect(next);

        if (!connected) {
            (*errorLog) << "\tfailed connect to " << next.getServer() << std::endl;
            continue;
        }

        factory->setFrom(next);

        (*errorLog) << "\t*Request " << next.getUri() << std::endl;
        if (!fetcher.request(next)) {
            (*errorLog) << "\t\tfailed" << std::endl;
            continue;
        }

        std::ofstream tmp(options.m_sTmpFilePath.c_str());

        CHeaderParser header;
        (*errorLog) << "\t\tget header " << std::endl;
        unsigned int status(fetcher.getHeader(header, tmp));
        if ((status != 200) && (status != 301) && (status != 302) && (status != 303)) {
            (*errorLog) << "\t\t\tfailed error = " << status << std::endl;
            (*errorLog) << "\t\t\tfrom server " << next.getServer() << std::endl;
            (*errorLog) << "\t\t\t**************************************" << std::endl;
            for (CHeaderParser::CIterator i = header.begin(); i != header.end(); ++i) {
                (*errorLog) <<"\t\t\t"<< i.headerKey() << " = " << i.headerValue() << std::endl;
            }
            (*errorLog) << "\t\t\t**************************************" << std::endl;
            ++http_errors;
            continue;
        }
        
        if ((status == 301) || (status == 302) || (status == 303)) {
            std::string loc;
            if (header.getField("Location", loc)) {
                (*errorLog) << "\t\t\tredirected to " << loc << std::endl;
               factory->pushLink(loc);
            }
            continue;
        }

        std::string cookies(""),
                    ext("html");

        header.getCookies(cookies);
        header.getExtension(ext);
        next.setCookie(cookies);

        factory->setFrom(next);

        if((!CLinkExtractor<int>::isParse(ext))&&(!options.m_bSavePages))
            continue;

        (*errorLog) << "\t*Get response" << std::endl;

        bool res(fetcher.getResponse(tmp));

        if (!res) {
            (*errorLog) << "\t\tfailed" << std::endl;
            continue;
        }
        tmp.close();


        std::string filepath("");


        if(options.m_bSavePages){
            if (!files.createPath(next.getServer(), next.getUri(), ext, filepath)) {
                (*errorLog) << "\t\tcouldnt create dir for " << filepath << std::endl;
                continue;
            }

            (*errorLog) << "\t\twrite to " << filepath << std::endl;
            try {
                
                if (boost::filesystem::exists(filepath)) {
                    boost::filesystem::remove(filepath);
                }

                boost::filesystem::copy_file(options.m_sTmpFilePath, filepath);
            } catch (...) {
                (*errorLog) << "\t\t\tcoudnt copy " << options.m_sTmpFilePath << " to " << filepath << std::endl;
                continue;
            }
        }else{
            filepath    =   options.m_sTmpFilePath;
        }        

        if (!CLinkExtractor<int>::isParse(ext)) {
            continue;
        }
	
        (*errorLog) << "\t*Parse " << filepath << std::endl;
        std::ifstream f(filepath.c_str());
        
        CLinkExtractor<ILinkFactory> extractor(*factory);
        extractor.extract(f);
    }

    if(options.m_bSaveHistory){

        if (not work_front.IsFutureEmpty()) {
            std::ofstream   futurelinks_endfile(options.m_sFutureLinksPath.c_str());
            (*errorLog) << "saving future links to " << options.m_sFutureLinksPath << std::endl;
            work_front.writeFutureLinks(futurelinks_endfile);
            futurelinks_endfile.close();
        }


        if (not work_front.IsUsedEmpty()) {
            std::ofstream   usedlinks_endfile(options.m_sUsedLinksPath.c_str());
            (*errorLog) << "saving used links to " << options.m_sUsedLinksPath << std::endl;
            work_front.writeUsedLinks(usedlinks_endfile);
            usedlinks_endfile.close();
        }
        
    }
    
    (*errorLog) << "*Stop working release resourses" << std::endl;
    if (boost::filesystem::exists(options.m_sTmpFilePath)) {
        boost::filesystem::remove(options.m_sTmpFilePath);
    }

    if(isNeedDelete) {
        delete errorLog;
    }

    delete factory;   
    
    ne_sock_exit();

    return 0;
}
