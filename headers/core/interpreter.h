#ifndef interpreter_H_INCLUDED
#define interpreter_H_INCLUDED
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <vector>
#include "build.h"
#include "modules/functions.h"
#include "interface/interpreter_interface.h"
#include "corax_virtual_machine.h"

using mathNode::number_type;
struct interpreterOops : public exception
{
	interpreterOops(std::string inf);
	const char* what();

};







class interpreter;


namespace CoraxVM
{

	class Corax_program_builder_module
	{
	private:
		interpreter * _ptr;
		//Internal command for compiling tree objects to CoraxVM byte code
		void _rpn(node * nodePtr, CoraxVM::corax_program * prgm);
	public:
		bool create_program(interface::corax_program*);
		Corax_program_builder_module(interpreter * ptr);
	};
};

rootNode buildEntry1(interpreter * parentInterpreter);



class interpreter : public interface::interpreter_interface
{
    friend rootNode buildEntry1(interpreter * parentInterpreter);
	friend CoraxVM::Corax_program_builder_module;
	char * expression;
	unsigned short expressionLength;
	unsigned short startOperatorPos; //contains the index in the tokens array where the starting operator is located
	operators::operators_interface* _operators;


	std::vector<baseToken*> tokens;

	rootNode root;
	math_func::function_interface* current_functions;
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
	void setOperator(operators::operators_interface* operators);
	void setMemory(memory* mem);
	void setFunction(math_func::function_interface* functions);
	bool interpret();
	number_type exec();
	void set(const char * expression, short lenght);
	interpreter();
	~interpreter();
	
};


#endif
