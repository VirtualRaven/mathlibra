#ifndef interpreter_H_INCLUDED
#define interpreter_H_INCLUDED
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <vector>
#include "build.h"
#include "functions.h"
using mathNode::number_type;
struct interpreterOops : public exception
{
	interpreterOops(std::string inf);
	const char* what();

};







class interpreter;



class buildTree
{
	interpreter* parentInterpreter;
	rootNode root;
	bool built;

public:
	buildTree(interpreter * tgt);
	~buildTree();
	rootNode getTree();
	bool buildEntry1();
};



class interpreter
{

	char * expression;
	unsigned short expressionLength;
	unsigned short startOperatorPos; //contains the index in the tokens array where the starting operator is located
	operators::allOperators operators;

	friend buildTree;
	std::vector<baseToken*> tokens;

	rootNode root;
	math_func::function_interface current_functions;
	memory *mem;
	bool rootEmpty;

	void emptyRoot();
	void destroyTokens();
    void stripSlashes();
	void freeExpression();
	void allocExpression(short lenght);
	bool lexicalAnalys();
	bool buildSyntaxTree();



public:
	/*Warning the interpeter does not copy the memory object.
	Therefore must the pointer remain valid throug the lifetime of the interpreter object*/
	void setMemory(memory* mem);
	bool interpret();
	number_type exec();
	void set(const char * expression, short lenght);
	interpreter();
	~interpreter();
};

#endif
