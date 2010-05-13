#ifndef _EXTENSIONS_H_
#define _EXTENSIONS_H_
#include <string>

namespace misc
{

const std::string TxtEdgesExt		=	"tge";
const std::string TxtVerticiesExt	=	"tgv";
const std::string DbEdgesExt		=	"dge";
const std::string DbVerticiesExt	=	"dgv";
const std::string DictExt		=	"bdb";
const std::string GraphVizExt		=	"dot";
const std::string GnuPlotExt		=	"dat";
const std::string ArffExt		=	"arff";
const std::string SvgExt		=	"svg";
const std::string GnuplotScriptExt	=	"p";

std::string addExt(const std::string &file_path,const std::string &ext);
std::string generateDictFile(const std::string &file_path);
std::string generateVertexDbFile(const std::string &file_path);
std::string generateEdgeDbFile(const std::string &file_path);
std::string generateVertexTxtFile(const std::string &file_path);
std::string generateEdgeTxtFile(const std::string &file_path);
std::string generateGraphVizFile(const std::string &file_path);
std::string generateGnuPlotFile(const std::string &file_path);
std::string generateArffFile(const std::string &file_path);
std::string generateSvgFile(const std::string &file_path);
std::string generateGnuplotScriptFile(const std::string &file_path);
size_t      findExt(const std::string &file_name);
std::pair<std::string,std::string>   cutExt(const std::string &file_name);
}

#endif //_EXTENSIONS_H_
