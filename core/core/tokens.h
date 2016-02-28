
#ifndef TOKENS_H_INCLUDED
#define TOKENS_H_INCLUDED

#include "core/type_interface.h"
#include "mathNode.h"
#include "tree.h"
#include "modules/operators.h"
#include "modules/functions.h"
#include "main.h"
#include "function_obj.h"
using tree::tokenType;
using interface::type;
/**
 *@namespace token Contains all token types. Tokens are used to identefy parts of strings.
*/
namespace token
{
/**
	Base class for tokens.
	The base class is an abstract interface class for tokens.
 */
 struct baseToken
{
	size_t startPos; /**< The starting position of an token in the string. @note This index is broken in the current revision*/ 
	size_t endPos;  /**< The ending position of an token in the string*/
	tokenType type; /**< The type of token @see tokenType. */

	virtual tree::nodeDataInterface* node()=0; /**< Constructor for nodeDataInterface object. @return nodeDataInterface pointer to the data coresponding to the type. */
	virtual bool  hasNode(); /**< Checks if token has an node type. @return true if node() returns an valid object and false if the token has no node object. */
	baseToken(); 
	virtual ~baseToken();

};

/**
 * Operator token.
 * The class for all operators. 
 */
struct operatorToken : public baseToken, operators::interpreter_operator
{
	tree::nodeDataInterface * node();
	bool  hasNode();
	operatorToken(operators::generic_oper_ptr opr, char symbol, short wheight); /**< @param opr A pointer to an operator. 
										         @param symbol A char that represents the operator, for example '+' for the addition operator. 
											 @param wheight The weight of the operator, the higher weight the higher priority the operators is given. */
	operatorToken(operators::interpreter_operator opr); /**< Copy constructor. */
	operatorToken();
};


/**
 * Parentheses token.
 *  This class stores paratheses signs as '(' and ')'. 
 */
struct parenthesesToken : public baseToken
{

	size_t opposit; 

	bool isOppening(); /**< @return True if the token represents '(' else false. */
	bool  hasNode();
	tree::nodeDataInterface * node();
	parenthesesToken(size_t startPos, size_t endPos);
	parenthesesToken();

};

/**
 * Token representing values. 
 * Values in the input string are stored in this token. All values are converted to doubles.
 */

struct valueToken : public baseToken
{
	interface::type_ptr value; /**< The value of the token, for example 1 or 4e100 */

	tree::nodeDataInterface* node();
	bool  hasNode();
	valueToken(size_t startPos, size_t endPos);
	valueToken();
};
/**
 * Token representing variables.
 * All variables are stored inside this token.
 */
struct variableToken : public baseToken
{
        math_func::function_interface* func;
	std::string variableName; /**< The name of the variable. Variable names can be an single character or an small word. No numbers or special characters are allowed*/
	memory::memory *mem; /**< Pointer to the memory module. The pointer points to the currently used memory module. This pointer is used evaluate the variable using the provided variableName. */
	bool _stack; /**< Stackable. Signifies  if the variable should be put on the stack. Used for the corax virtual machine. */
	tree::nodeDataInterface* node();
	bool  hasNode();	
	variableToken(size_t startPos, size_t endPos, memory::memory* mem, math_func::function_interface* func);
	variableToken();
        variableToken(const variableToken&);
        variableToken operator=(const variableToken&); 


};

enum ptr_type_enum {std,gen,usr};
/**
 *  Token representing an token.
 * All function type nodes are represented by this token.
 */
struct funcToken : public baseToken
{
       
         
	typedef interface::type*(*generalFuncPtr)(tree::node_base*); /** @typedef Special pointer for multi argument functions. Used by FUNCTION_TREE type nodes */
        typedef function_obj::interpreted_func* usr_ptr;
	union
	{
		generalFuncPtr gptr;
                usr_ptr uptr;    
                
	};
	unsigned short baseWheight; /**< Weight of the operator. Higher weight means higher execution priority.  */
	tree::nodeDataInterface* node();
	bool  hasNode();
	funcToken(size_t startPos, size_t endPos, generalFuncPtr ptr);
        funcToken(size_t startPos,size_t endPos, usr_ptr ptr);
	funcToken();
private:
            ptr_type_enum ptr_type;
};
}
#endif // TOKENS_H_INCLUDED
