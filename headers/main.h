#ifndef MAIN_H_INCLUDED
#define MAI_H_INCLUDED
//#define DEBUG
typedef double number_type;
#define DEBUG
#ifdef DEBUG
#define LEXICAL_ANANALYSIS_DEBUG
#define SYNTAX_TREE_DEBUG_INFO
#define STRUCTUAL_INTEGRITY_TEST
#define DEBUG_CORAX_INS
#endif //DEBUG
namespace util
{

	void buildInfo();
	void help();
	
}
#ifdef GIT_COMMIT_HASH
extern const char * git_version;
#endif //GIT_COMMIT_HASH
#endif

