
#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>

#include <parserbase.h>

#include <system.h>



namespace misc
{

bool createDirectory(const std::string &path) {


        bool ret(true);
        std::string path_(path),
                    buff("");
        
        CParserBase parser("/");
        parser.parseString(path_);
        size_t i(1);
        while (i != parser.size() && ret) {
            parser.getStart(i, buff);
            if (boost::filesystem::exists(buff)) {
                ret = boost::filesystem::is_directory(buff);
            } else {
                try {
                    boost::filesystem::create_directory(buff);
                } catch (...) {
                    ret = false;
                }
            }
            ++i;
        }
            

        
        return ret;
}


}
