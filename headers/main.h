#ifndef MAIN_H_INCLUDED
#define MAI_H_INCLUDED
#define DEBUG
#ifdef DEBUG

#ifdef DEBUG 
#define LEXICAL_ANANALYSIS_DEBUG
//#define SYNTAX_TREE_DEBUG_INFO
//#define STRUCTUAL_INTEGRITY_TEST
#endif //DEBUG
namespace util
{

	void buildInfo();
	void help();
}
#endif
#endif