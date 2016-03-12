#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED
#include <vector>
#include <string>
using std::vector;
using std::string;

enum struct RETURN_VAL : short { SUCCESS =0, MEM_ERROR=1, EXCEPTION=2, CRITICAL_EXCEPTION=3,OTHER=4 }; 
RETURN_VAL  runTest(vector<string> testExpression);
#endif
