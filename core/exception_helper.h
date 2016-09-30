/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef EXCEPTION_HELPER_INCLUDED
#define EXCEPTION_HELPER_INCLUDED
#include "exception.h"
#include <cstring>
//* All exceptions that can be thrown is decalared in this enum
enum EXCEPTION {
	MEMORY_LEAK_ASSERT,
	SUBSCRIPT_OPERATOR_OUT_OF_BOUNDS,
	TREE_BUILD_FAILED,
	SYNTAX_UNMATCHED_CLOSING_PARANTHESES,
	SYNTAX_UNMATCHED_OPENING_PARANTHESES,
	SYNTAX_UNMATCHED_CLOSING_SQUARE_PARANTHESES,
	SYNTAX_EXP_AFTER_E,
	SYNTAX_ASSIGMENT_NEEDS_VAR,
	SYNTAX_EXPECTED_RH_OPERAND,
	SYNTAX_EXPECTED_LH_OPERAND,
	SYNTAX_DUPLICATED_OPERATORS_RH,
	SYNTAX_DUPLICATED_OPERATORS_LH,
	SYNTAX_EXPECTED_FUNC_ARG,
	VARIABLES_DISABLED_BUT_USED,
	UNKNOWN_CHAR_STR,
	CANT_FIND_STARTING_POINT,
	EXEC_UNFINISHED_EXPR,
	EMPTY_EXPR,
	CANT_CPY_OBJ,
	FUNC_OPR_NEED_EXPR,
	FUNC_OPR_NO_REDEFINE,
	FUNC_OPR_NO_REDEFINE_SYSTEM,
	FUNC_OPR_NEED_FUNC_NAME,
        FUNC_MAP_EXPR_NOT_NUMBER,
	MEM_VAR_NOT_FOUND,
	MEM_ALTER_CONST_VAR,
	MEM_INDEX_OUT_OF_BOUNDS,
	FUNC_NO_CONV_FUNCTOR_VOID_P,
	TREE_BARE_NODE_NO_CPY,
	TREE_UNITILIZED_COUNTER_POINTER,
	TREE_CPY_FAILED_NO_DATA,
	TREE_FAILED_CONTEXT_NO_DATA,
	TREE_FORWARDED_EXCEPTION,
	PLUGIN_FAILED_SEARCH_FOLDER,
	PLUGIN_LOAD_FAILED,
	PLUGIN_ENTRY_NOT_DEFINED,
	PLUGIN_DYNAMIC_ERROR,
	BUILD_UNKNOWN_ERROR,
	NODE_VAR_NO_REDECLARE_FUNC,
	NODE_NO_FUNC_DEF_WITHOUT_FUNCTION_INTERFACE,
	NODE_NO_FUNC_REDEFINE,
	NODE_UNEXPECTED_NULL_POINTER,
	NODE_DUMMY_NODE_EXEC,
	NODE_EXPECTED_ARGUMENT_FOR_FUNC,
	NODE_EVAL_UNDEFINED_FUNC,
	TOKEN_NO_MEM_MODULE,
	TOKEN_FUNCTION_NULL_POINTER,
    	REALLOC_NO_RULE,
    	USER_DEFINED_FUNCTION_TAKES_NUMBER,
	PTR_PROTECT_MOVED_FREED_PTR,
	SYNTAX_EXPECTED_END_OF_STRING,
	EMPTY_MAT_INIT,
	DIFFERENT_N_MAT_INIT,
	DOUBLE_REQUIRED_MAT_INIT,
	MATRICE_MUST_BE_DOUBLE_MAT_INIT,
	MATRICE_NEW_LINE_SYMBOL,
	SQUARE_BRACKET_MISSMATCH,
	MATRICE_NEW_LINE_UNALLOWED,
	MATRICE_NEW_LINE_NOT_LAST,
	MATRICE_SYNTAX_DISABLED,
	MUL_OPR_INVALID_ARGS,
	MAP_FUNCTION_NOT_FOUND,
	MAP_FUNCTION_NOT_NUMER_TYPE,
        RANGE_FUNCTION_INVALID_MATRIX
	};

//* Lists all possible exception owners
enum EXCEPTION_TYPE {
		INTERPRETER,
		STD_OPERATORS,
		MEMORY,
		FUNCTION,
		TREE,
		PLUG_IN,
		BUILD,
		NODE,
		TOKEN,
		PTR_PROTECT,
		WRAPPER
};

extern char __exception_buffer[128];


	//* Property class which holds properties for EXCEPTION_TYPE
	template <EXCEPTION_TYPE T> struct exception_type_prop{};

	//*Property class which holds properties for EXCEPTION
	template<EXCEPTION T> struct exception_property{};

	template<bool assert> struct StaticAssert{};
        template<> struct StaticAssert<true>
	{
		enum {assert };
	};	


//*The main exception function. Don't use this function directly, all exception owners implements
//their own version of this one. The function will fetch the exception data from the exception 
//declaration and  raise  an exception using those properties declared.
template<EXCEPTION T,EXCEPTION_TYPE X,short assert=StaticAssert<exception_property<T>::owner==X>::assert>  void __mathlibra__raise()
{
	//Terribly sorry that we had to end it likes this :/
	throw exception(exception_type_prop<exception_property<T>::owner>::str,
			exception_property<T>::info,
			exception_property<T>::critical,
			T);
}

//Exception function to be used when the message to throw can not be determined at compiletime
//Exceptions thrown by this function has to implement all properties except info.
template<EXCEPTION T,EXCEPTION_TYPE X,short assert=StaticAssert<exception_property<T>::owner==X>::assert> inline void __mathlibra__runtime__raise(const char * message)
{
	strncpy(&__exception_buffer[0], message, 127);

	//Terribly sorry that we had to end it likes this :/
	throw exception(exception_type_prop<exception_property<T>::owner>::str,
			&__exception_buffer[0],
			exception_property<T>::critical,
			T);
}

//The following is property types that is used when declaring exceptions and exception_types
#define  str_property static constexpr const char *
#define  bool_property  static constexpr bool 
#define  uint_property static  unsigned int 
#define  owner_property static constexpr EXCEPTION_TYPE


#define declare_exception(property) template<> struct exception_property<property> 
//Macro to declare an new exception
//usage:
//	declare_exception(EXCEPTION_ENUM)
//	{
//		str_property info = "The string describing the exact exception"
//		bool_property critical = false 
//		owner_property owner = EXCEPTION_TYPE_ENUM 
//	};
//
//	--critical-- 
//	critical should be set to true if the exception is an fatal exception. Throwing an critical
//	exception signals that the library is no longer in an valid state and that this exception should
//	be reported.
//
//	--owner-- 
//	owner should be set to the EXEPTION_TYPE enum that owns the exception. This property declares
//	which functions can cast the exception. For example an exception with owner set to MEMORY
//	can only be cast by memoryOops. Trying to cast it with interpreterOops will cause an compelation
//	failure.


#define declare_exception_owner(property) template<> struct exception_type_prop<property> 
//Macro to declare a new exception_owner
//usage:
//	declare_exception_owner(EXCEPTION_TYPE_ENUM)
//	{
//		str_property str = "A string describing the exception type"
//	};
//	

/***************************************************************
*              LIST OF EXCEPTION OWNERS
**************************************************************/

declare_exception_owner(INTERPRETER)
{
	str_property str = "Intepreter Oops";
};

declare_exception_owner(STD_OPERATORS)
{
	str_property str = "Std operator Oops";
};

declare_exception_owner(MEMORY)
{
	str_property str = "Memory Oops";
};

declare_exception_owner(FUNCTION)
{
	str_property str = "Function Oops";
};
declare_exception_owner(TREE)
{
	str_property str = "Tree Oops";
};
declare_exception_owner(PLUG_IN)
{
	str_property str = "Plugin-in Oops";
};
declare_exception_owner(BUILD)
{
	str_property str = "Build Oops";
};
declare_exception_owner(NODE)
{
	str_property str ="Node Oops";
};
declare_exception_owner(TOKEN)
{
	str_property str = "Token Oops";
};
declare_exception_owner(WRAPPER)
{
	str_property str = "Wrapper Oops";
};


declare_exception_owner(PTR_PROTECT)
{
	str_property str = "Ptr protect Oops (Severe bug)";
};
/***************************************************************
*              LIST OF EXCEPTIONS
**************************************************************/
declare_exception(PTR_PROTECT_MOVED_FREED_PTR)
{
	str_property info = "Tried to move released pointer";
	bool_property critical = true;
	owner_property owner = PTR_PROTECT;
};
declare_exception(MEMORY_LEAK_ASSERT)
{
	str_property info = "ASSERT FAILURE,possible memory leak in program";
	bool_property critical = true;
	owner_property owner = INTERPRETER;
};

declare_exception(SUBSCRIPT_OPERATOR_OUT_OF_BOUNDS)
{
	str_property info ="Token subscript operator out of bounds";
	bool_property critical = true;
	owner_property owner = INTERPRETER;
};


declare_exception(TREE_BUILD_FAILED)
{
	str_property info ="failed to construct syntax tree";
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};

declare_exception(SYNTAX_UNMATCHED_OPENING_PARANTHESES)
{
	str_property info ="Syntax error: found unmatched (";
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};
declare_exception(SYNTAX_UNMATCHED_CLOSING_SQUARE_PARANTHESES)
{
	str_property info ="Syntax error: found unmatched ]";
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};
declare_exception(SYNTAX_UNMATCHED_CLOSING_PARANTHESES)
{
	str_property info ="Syntax error: found unmatched )";
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};

declare_exception(SYNTAX_EXP_AFTER_E)
{
	str_property info ="Syntax error: expected expoent after e";
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};
declare_exception(SYNTAX_ASSIGMENT_NEEDS_VAR)
{
	str_property info ="Syntax error: Assigment operator requires an variable on left hand side of =";
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};

declare_exception(VARIABLES_DISABLED_BUT_USED)
{
	str_property info ="Found variable in expression but no memory unit is assignet to the interpreter";
	bool_property critical = true;
	owner_property owner = INTERPRETER;
};

declare_exception(UNKNOWN_CHAR_STR)
{
	str_property info ="Found unknown character in string";
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};

declare_exception(CANT_FIND_STARTING_POINT)
{
	str_property info ="Sorry, can't understand expression";
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};
declare_exception(EXEC_UNFINISHED_EXPR)
{
	str_property info ="Tried to execute unfinished expression";
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};

declare_exception(EMPTY_EXPR)
{
	str_property info ="Passed empty expression to interpreter";
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};
declare_exception(CANT_CPY_OBJ)
{
	str_property info ="Intepreter object can't be copied";
	bool_property critical = true;
	owner_property owner = INTERPRETER;

};

declare_exception(FUNC_OPR_NEED_EXPR)
{
	str_property info ="Expected expression on right hand side of : operator";
	bool_property critical = false;
	owner_property owner = STD_OPERATORS;

};
declare_exception(FUNC_OPR_NO_REDEFINE)
{
	str_property info ="Can't redine already defined function";
	bool_property critical = false;
	owner_property owner = STD_OPERATORS;

};
declare_exception(FUNC_OPR_NO_REDEFINE_SYSTEM)
{
	str_property info ="Can't redefine built in function";
	bool_property critical = false;
	owner_property owner = STD_OPERATORS;

};
declare_exception(FUNC_OPR_NEED_FUNC_NAME)
{
	str_property info ="Expected function name on lefthand side of : operator";
	bool_property critical = false;
	owner_property owner = STD_OPERATORS;

};

declare_exception(MEM_VAR_NOT_FOUND)
{
	str_property info ="Requested variable not found";
	bool_property critical = false;
	owner_property owner = MEMORY;

};


declare_exception(MEM_ALTER_CONST_VAR)
{
	str_property info ="Tried to alter constant variable";
	bool_property critical = false;
	owner_property owner = MEMORY;

};
declare_exception(MEM_INDEX_OUT_OF_BOUNDS)
{
	str_property info ="Requested index out of bounds";
	bool_property critical = false;
	owner_property owner = MEMORY;

};

declare_exception(FUNC_NO_CONV_FUNCTOR_VOID_P)
{
	str_property info ="Can't converto functor to void *";
	bool_property critical = true;
	owner_property owner = FUNCTION;

};

declare_exception(TREE_BARE_NODE_NO_CPY)
{
	str_property info ="bare node can not be copied";
	bool_property critical = true;
	owner_property owner = TREE;

};

declare_exception(FUNC_MAP_EXPR_NOT_NUMBER)
{
	str_property info ="Map function expression must result in a number";
	bool_property critical = false;
	owner_property owner = FUNCTION;

};

declare_exception(TREE_UNITILIZED_COUNTER_POINTER)
{
	str_property info ="Unitilized copy counter pointer";
	bool_property critical = true;
	owner_property owner = TREE;
};

declare_exception(TREE_CPY_FAILED_NO_DATA)
{
	str_property info ="Failed to copy tree structure, data pointer empty";
	bool_property critical = true;
	owner_property owner = TREE;
};


declare_exception(TREE_FAILED_CONTEXT_NO_DATA)
{
	str_property info ="Failed to aquire context, data pointer empty";
	bool_property critical = true;
	owner_property owner = TREE;
};

declare_exception(TREE_FORWARDED_EXCEPTION)
{
	bool_property critical = false;
	owner_property owner = TREE;
};


declare_exception(PLUGIN_FAILED_SEARCH_FOLDER)
{
	str_property info ="Plugin folder not found";
	bool_property critical = true;
	owner_property owner = PLUG_IN;
};

declare_exception(PLUGIN_LOAD_FAILED)
{
	str_property info ="Could not load shared library";
	bool_property critical = true;
	owner_property owner = PLUG_IN;
};
declare_exception(PLUGIN_ENTRY_NOT_DEFINED)
{
	str_property info ="Plugin has no defined PLUGIN_ENTRY";
	bool_property critical = true;
	owner_property owner = PLUG_IN;
};
declare_exception(PLUGIN_DYNAMIC_ERROR)
{
	bool_property critical = true;
	owner_property owner = PLUG_IN;
};
declare_exception(BUILD_UNKNOWN_ERROR)
{
	str_property info = "Unknown error occured in intepretetion of expression, sorry :/ ";
	bool_property critical = true;
	owner_property owner = BUILD;
};

declare_exception(NODE_VAR_NO_REDECLARE_FUNC)
{
	str_property info = "Can't redeclare defined variable as function";
	bool_property critical = false;
	owner_property owner = NODE;
};

declare_exception(NODE_NO_FUNC_DEF_WITHOUT_FUNCTION_INTERFACE)
{
	str_property info = "Can't define function without an function_interface instance";
	bool_property critical = true;
	owner_property owner = NODE;
};

declare_exception(NODE_NO_FUNC_REDEFINE)
{
	str_property info = "Can't redefine function";
	bool_property critical = false;
	owner_property owner = NODE;
};

declare_exception(NODE_UNEXPECTED_NULL_POINTER)
{
	str_property info = "Unexpected null pointer in syntax tree";
	bool_property critical = true;
	owner_property owner = NODE;
};

declare_exception(NODE_DUMMY_NODE_EXEC)
{
	str_property info = "Tried to execute dummy node";
	bool_property critical = false;
	owner_property owner = NODE;
};

declare_exception(NODE_EXPECTED_ARGUMENT_FOR_FUNC)
{
	str_property info = "Expected argument for function call";
	bool_property critical = false;
	owner_property owner = NODE;
};

declare_exception(NODE_EVAL_UNDEFINED_FUNC)
{
	str_property info = "Tried to evaluate undefine function";
	bool_property critical = false;
	owner_property owner = NODE;
};

declare_exception(TOKEN_NO_MEM_MODULE)
{
	str_property info = "Tried to create variable without assigning memory module";
	bool_property critical = true;
	owner_property owner = TOKEN;
};

declare_exception(TOKEN_FUNCTION_NULL_POINTER)
{
	str_property info = "Function node missing pointer to actual function";
	bool_property critical = true;
	owner_property owner = TOKEN;
};

declare_exception(SYNTAX_EXPECTED_RH_OPERAND)
{
	str_property info = "Expected right hand side operand";
	bool_property critical = false;
	owner_property owner = BUILD;
};

declare_exception(SYNTAX_EXPECTED_LH_OPERAND)
{
	str_property info ="Expected left hand side operand";
	bool_property critical = false;
	owner_property owner = BUILD;
};

declare_exception(SYNTAX_DUPLICATED_OPERATORS_RH)
{
	str_property info = "Found extra operator before current operator";
	bool_property critical = false;
	owner_property owner = BUILD;
};

declare_exception(SYNTAX_DUPLICATED_OPERATORS_LH)
{
	str_property info = "Found extra operator after current operator";
	bool_property critical = false;
	owner_property owner = BUILD;
};
	
declare_exception(SYNTAX_EXPECTED_FUNC_ARG)
{
	str_property info = "Expected argument after function name";
	bool_property critical = false;
	owner_property owner = BUILD;
};

declare_exception(REALLOC_NO_RULE)
{
        str_property info = "No rule found for reallocation current object";
        bool_property critical = true;
        owner_property owner = TREE;
};

declare_exception(USER_DEFINED_FUNCTION_TAKES_NUMBER)
{   
    str_property info = "A user defined function can only take single number";
    bool_property critical = false;
    owner_property owner = NODE;


};

declare_exception(SYNTAX_EXPECTED_END_OF_STRING)
{
	str_property info = "Expected ending \" at end of string";
	bool_property critical = false;
	owner_property owner = INTERPRETER;

};

declare_exception(EMPTY_MAT_INIT)
{
	str_property info = "Cannot create an empty matrix";
	bool_property critical = false;
	owner_property owner = FUNCTION;

};
declare_exception(DIFFERENT_N_MAT_INIT)
{
	str_property info = "All matrices in bracket initilization must be of same type";
	bool_property critical = false;
	owner_property owner = FUNCTION;

};
declare_exception(DOUBLE_REQUIRED_MAT_INIT)
{
	str_property info = "All element in bracket initilization must be of double matrix or double type";
	bool_property critical = false;
	owner_property owner = FUNCTION;

};
declare_exception(MATRICE_MUST_BE_DOUBLE_MAT_INIT)
{
	str_property info = "Bracket matrix initilization can only create double matrix";
	bool_property critical = false;
	owner_property owner = FUNCTION;
};

declare_exception(MATRICE_NEW_LINE_SYMBOL)
{
	str_property info = "Matrix new line operator '|' can only be used inside an bracket initilization";
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};
declare_exception(SQUARE_BRACKET_MISSMATCH)
{
	str_property info ="Missing opening square bracket";
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};
declare_exception(MATRICE_NEW_LINE_UNALLOWED)
{
	str_property info = "Matrix new line operator '|' must be preceded by at least matrix element"; 
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};
declare_exception(MATRICE_NEW_LINE_NOT_LAST)
{
	str_property info = "Matrix new line operator '|' must have element after itself"; 
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};
declare_exception(MATRICE_SYNTAX_DISABLED)
{
	str_property info = "Matrices has been disabled"; 
	bool_property critical = false;
	owner_property owner = INTERPRETER;
};
declare_exception(MUL_OPR_INVALID_ARGS)
{
	str_property info = "Invalid matrix sizes of multiplication operands";
	bool_property critical = false;
	owner_property owner = STD_OPERATORS;
};
declare_exception(MAP_FUNCTION_NOT_FOUND)
{
	str_property info = "Function to map over does not exist";
	bool_property critical = false;
	owner_property owner = WRAPPER;
};
	
declare_exception(MAP_FUNCTION_NOT_NUMER_TYPE)
{
	str_property info = "Map function did not return number";
	bool_property critical = false;
	owner_property owner = WRAPPER;
};

declare_exception(RANGE_FUNCTION_INVALID_MATRIX)
{
    str_property info ="Range function only accepts 1x1,1x2 or 1x3 matrices";
    bool_property critical = false;
    owner_property owner = FUNCTION;
};
	
#endif //EXCEPTION_HELPER_INCLUDED
