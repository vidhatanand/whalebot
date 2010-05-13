//
// File:   filesystem.h
// Author: hotdox
//
// Created on 11 Апрель 2008 г., 21:48
//
//move filesystem operation from
//unix::std commands to boost::filesystem

#ifndef _SYSTEM_H
#define	_SYSTEM_H

#include <string>
#include <deque>
#include "extensions.h"

namespace misc
{

class DirectoryIterator
{
public:
	const bool set(const std::string &start_path);
	//TODO take into consideration access_type to file
	DirectoryIterator(const std::string &start_path = "");
        const bool iterate(std::string &next_node, bool &is_dir);
	void stop();
private:
	std::deque<std::string> work_frontier;
};

std::string generateFileName(const std::string &name_base,const int &number,const int &number_length);
std::string generateFileNameWthExt(const std::string &name_base,const int &number,const int &number_length);
std::string generateUniqFileNameWthExt(const std::string &name_base,int &number,const int &number_length);


bool createDirectory(const std::string &path);

class FileBuffer
{
public:
                            FileBuffer(const std::string &need_buffer_dir,const std::string &need_buffer_name="buff",const int &need_max_size = 5);
	const std::string   newFile(const std::string &extension = "");
	void                clear();
                            ~FileBuffer();
private:
	int         file_counter,
                    max_size;
	std::string buffer_dir;
	std::string buff_name;
};

}
#endif	/* _SYSTEM_H */

