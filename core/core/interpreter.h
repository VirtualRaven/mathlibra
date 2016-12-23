/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef interpreter_H_INCLUDED

#define interpreter_H_INCLUDED
#include <stdlib.h>
#include <string>
//#include <cstring>
//#include <cstdlib>
#include <iostream>
#include <stack>
#include <vector>
#include <cstring>
#include "build.h"
#include "tree.h"
#include "modules/functions.h"
#include "interface/interpreter_interface.h"
#include "corax_virtual_machine.h"
#include "core/function_obj.h"
#include "core/token_list.h"
using tree::rootNode;
using tree::node;
using function_obj::interpreted_func;

/**
 * @namespace debug Contains function which helps during program debugging
 */


class interpreter;

/** 
 * @namespace CoraxVM Virtual machine name-space.  Contains functions and classes related to the virtual machine runtime environment. 
 */
#ifdef ENABLE_CORAX
namespace CoraxVM
{
	/**
	 * Helper class  to compile an abstract syntax tree into byte code.
	 * The byte code created an be run in the corax virtual machine.
	 */
	class Corax_program_builder_module
	{
	private:
		interpreter * _ptr;
		//Internal command for compiling tree objects to CoraxVM byte code
		void _rpn(node * nodePtr, CoraxVM::corax_program * prgm);
	public:
		bool create_program(interface::corax_program*); /**< Compiles tree into byte code. */
		Corax_program_builder_module(interpreter * ptr); /**< Constructor for the helper class. @param ptr Pointer to the interpreter object which tree shall be used for the compilation. */
	};
};
#endif //ENABLE_CORAX
rootNode buildEntry1(interpreter * parentInterpreter);

/**
 * Interpreter class.
 * The class contains all major functions for implementing the interpreter. It serves as the main class implementing functionality from helper classes and the three main modules.
 */

class interpreter : public interface::interpreter_interface
{
    friend rootNode buildEntry1(interpreter * parentInterpreter);
#ifdef ENABLE_CORAX
	friend CoraxVM::Corax_program_builder_module;
#endif //ENABLE_CORAX
	char * expression;
	size_t expressionLength;
	size_t startOperatorPos; //contains the index in the tokens array where the starting operator is located
	operators::operators_interface* _operators;


	std::vector<token::baseToken*> tokens;

	rootNode root;
	math_func::function_interface* current_functions;
	memory::memory *mem;

	bool rootEmpty;

	void emptyRoot();
	void destroyTokens();
	void stripParanthese();
	void freeExpression();
	void allocExpression(size_t lenght);
        std::vector<token_list::item> lexicalAnalys();
	void buildSyntaxTree();
	interpreter(const interpreter& other);
	interpreter& operator=(const interpreter& other);
	interpreter& operator=(interpreter&& other);

public:
	/*Warning the interpeter does not copy the memory object.
	Therefore must the pointer remain valid throug the lifetime of the interpreter object*/
	void setOperator(operators::operators_interface* operators); /**< Load operator module. @param operators Pointer to the  operator module to load. @note does not copy the module, therefore the pointer must remain valid through the lifetime of the interpeter. */
	void setMemory(memory::memory* mem_); /**< Load memory module. @param mem_ Pointer to the module to load. @note does not copy the module, therefore the pointer must remain valid through the lifetime of the interpeter. */
	void setFunction(math_func::function_interface* functions); /**< Load function module. @param functions Pointer to the  module to load. @note does not copy the module, therefore the pointer must remain valid through the lifetime of the interpeter. */
	void interpret(); /**< Construct the abstract syntax tree.  @note The tree remains until the next call to interpret() or the destructor. Thus several calls can be made to exec cheaply after the construction.*/
        std::vector<token_list::item> extendedInterpret();
	interpreted_func createFunction(); /**< Same as interpret but exports the syntax tree as a interpreted_func object instead of saving it internally. @note interpreted_func objects runs in a seperate variable scope. Thus changeing a variable after creating the function does not affect the function. */
        interface::type_ptr  exec(); /**< Evaluate the abstract syntax tree. @return The result of the tree evaluation. */
	void set(const char * expression_, size_t lenght); /**< Sets the expression to interpret. @param expression_ Pointer to an c-style string. @param lenght the lenght of the string excluding any termination characters.*/
	interpreter();
	interpreter(interpreter&& other);

  	void optimize();	
	~interpreter();
	
};


#endif
