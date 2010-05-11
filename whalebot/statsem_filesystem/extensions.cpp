#include "extensions.h"
#include <string>

namespace misc
{
 std::string addExt(const std::string &file_path,const std::string &ext)
{
	if(!ext.empty()){
		return file_path+"."+ext;
	}
	else{
		return file_path;
	}

}
 std::string generateDictFile(const std::string &file_path) {
	return addExt(file_path,DictExt);
}
 std::string generateVertexDbFile(const std::string &file_path) {
	return addExt(file_path,DbVerticiesExt);
}

 std::string generateEdgeDbFile(const std::string &file_path) {
	return addExt(file_path,DbEdgesExt);
}

 std::string generateVertexTxtFile(const std::string &file_path) {
	return addExt(file_path,TxtVerticiesExt);
}

 std::string generateEdgeTxtFile(const std::string &file_path) {
	return addExt(file_path,TxtEdgesExt);
}
 std::string generateGraphVizFile(const std::string &file_path) {
	return addExt(file_path,GraphVizExt);
}
 std::string generateGnuPlotFile(const std::string &file_path) {
	return addExt(file_path,GnuPlotExt);
}

 std::string generateArffFile(const std::string &file_path) {
	return addExt(file_path,ArffExt);
}
 std::string generateSvgFile(const std::string &file_path) {
	return addExt(file_path,SvgExt);
}
 std::string generateGnuplotScriptFile(const std::string &file_path) {
	return addExt(file_path,GnuplotScriptExt);
}
 size_t	findExt(const std::string &file_name){
    size_t  ret(file_name.rfind('.')),
            slash_pos(file_name.rfind('/'));
    if((ret != std::string::npos)&&(slash_pos != std::string::npos)&&( ret < slash_pos)){
        ret =   std::string::npos;
    }
    return ret;
}
 std::pair<std::string,std::string> cutExt(const std::string &file_name){
	size_t		pos(findExt(file_name));
	std::string	ret1(file_name),
				ret2("");
	if(pos!=std::string::npos){
		ret1	=	file_name.substr(0,pos);
		ret2	=	file_name.substr(pos+1);
	}
	return std::make_pair(ret1,ret2);
}
}
