#include "system.h"
#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include "../statsem_string/parserbase.h"



namespace misc
{


	const bool DirectoryIterator::set(const std::string &start_path) {
		if (boost::filesystem::exists(start_path)) {
			work_frontier.push_back(start_path);
			return true;
		} else {
			return false;
		}
        }
	DirectoryIterator::DirectoryIterator(const std::string &start_path){
		set(start_path);
        }

        const bool DirectoryIterator::iterate(std::string &next_node, bool &is_dir) {
            bool ret(false);
            if (!work_frontier.empty()) {
                ret         =   true;
                is_dir      =   false;
                next_node   =   work_frontier.front();
                work_frontier.pop_front();                
                if (boost::filesystem::is_directory(next_node)) {
                    is_dir  =   true;
                    boost::filesystem::directory_iterator   i(next_node),
                                                            end;
                    while (i != end) {
                        std::string str_path( i->path().file_string());
                        work_frontier.push_back(str_path);
                        ++i;
                    }
                }
            }
            return ret;
        }
	void DirectoryIterator::stop(){
		work_frontier.clear();
	}

std::string generateFileName(const std::string &name_base,const int &number,const int &number_length){
	std::string str_number(boost::lexical_cast<std::string>(number));
	size_t      l_str_number(str_number.size());
	int         delta(number_length - l_str_number);

	for(int i=0; i<delta; ++i){
		str_number	=	'0' + str_number;
	}

	return name_base + str_number;
}

std::string generateFileNameWthExt(const std::string &name_base,const int &number,const int &number_length){
	std::pair<std::string,std::string> parse_file_name(cutExt(name_base));
	parse_file_name.first	=	generateFileName(parse_file_name.first,number,number_length);
	return addExt(parse_file_name.first,parse_file_name.second);
}

std::string generateUniqFileNameWthExt(const std::string &name_base,int &number,const int &number_length){
	std::string ret(generateFileNameWthExt(name_base,number,number_length));
	while(boost::filesystem::exists(ret)){
		++number;
		ret	=	generateFileNameWthExt(name_base,number,number_length);
	}
	return ret;
}

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
};


	FileBuffer::FileBuffer(const std::string &need_buffer_dir,const std::string &need_buffer_name,const int &need_max_size)
	:file_counter(0),max_size(need_max_size),buffer_dir(need_buffer_dir),buff_name(this->buffer_dir+'/' + need_buffer_name)
	{}
	const std::string FileBuffer::newFile(const std::string &extension){
            ++file_counter;
            return generateUniqFileNameWthExt(addExt(buff_name,extension),file_counter,max_size);
	}
	void FileBuffer::clear(){
		DirectoryIterator               di(buffer_dir);
		std::string			tmp;
		bool				b_tmp;
		while(di.iterate(tmp,b_tmp)){
			if(!b_tmp){
				boost::filesystem::remove(tmp);
			}
		}
		di.stop();

		di.set(buffer_dir);
		while(di.iterate(tmp,b_tmp)){
			if(tmp!=buffer_dir){
				boost::filesystem::remove(tmp);
			}
		}
	}
	FileBuffer::~FileBuffer(){
		clear();
	}


}
