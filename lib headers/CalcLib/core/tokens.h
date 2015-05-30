
#ifndef TOKENS_H_INCLUDED
#define TOKENS_H_INCLUDED

#include "mathNode.h"
#include "tree.h"
#include "modules/operators.h"
#include "main.h"
using mathNode::number_type;
using tree::tokenType;
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
	short startPos; /**< The starting position of an token in the string. @note This index is broken in the current revision*/ 
	short endPos;  /**< The ending position of an token in the string*/
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
	operatorToken(operators::operPtr opr, char symbol, short wheight); /**< @param opr an pointer to an operator. @param symbol An char that represents the operator, for example '+' for the addition operator. @param weight The weight of the operator, the higher weight the higher priority the operators is given. */
	operatorToken(operators::assigmentPtr assign, char symbol, short wheight); /**< @param assign an pointer to an assigment operator. @param symbol An char that represents the operator, for example '=' for the variable assignment operator. @param weight The weight of the operator, the higher weight the higher priority the operators is given. */
	operatorToken(operators::interpreter_operator opr); /**< Copy constructor. */
	operatorToken();
};


/**
 * Parentheses token.
 *  This class stores paratheses signs as '(' and ')'. 
 */
struct parenthesesToken : public baseToken
{

	short opposit; 

	bool isOppening(); /**< @return True if the token represents '(' else false. */
	bool  hasNode();
	tree::nodeDataInterface * node();
	parenthesesToken(short startPos, short endPos);
	parenthesesToken();

};

/**
 * Token representing values. 
 * Values in the input string are stored in this token. All values are converted to doubles.
 */

struct valueToken : public baseToken
{
	double value; /**< The value of the token, for example 1 or 4e100 */

	tree::nodeDataInterface* node();
	bool  hasNode();
	valueToken(short startPos, short endPos);
	valueToken();
};
/**
 * Token representing variables.
 * All variables are stored inside this token.
 */
struct variableToken : public baseToken
{

	std::string variableName; /**< The name of the variable. Variable names can be an single character or an small word. No numbers or special characters are allowed*/
	memory::memory *mem; /**< Pointer to the memory module. The pointer points to the currently used memory module. This pointer is used evaluate the variable using the provided variableName. */
	bool _stack; /**< Stackable. Signifies  if the variable should be put on the stack. Used for the corax virtual machine. */
	tree::nodeDataInterface* node();
	bool  hasNode();
	variableToken(short startPos, short endPos, memory::memory* mem);
	variableToken();
    variableToken(const variableToken&);
    variableToken operator=(const variableToken&); 


};

/**
 *  Token representing an token.
 * All function type nodes are represented by this token.
 */
struct funcToken : public baseToken
{

	typedef number_type(*funcPtr)(number_type); /**< @typedef The standard single argument function. Used by FUNCTION type nodes */
	typedef number_type(*generalFuncPtr)(tree::node_base*); /** @typedef Special pointer for multi argument functions. Used by FUNCTION_TREE type nodes */
	union
	{
		funcPtr ptr;
		generalFuncPtr gptr;
	};
	short baseWheight; /**< Weight of the operator. Higher weight means higher execution priority.  */
	tree::nodeDataInterface* node();
	bool  hasNode();
	funcToken(short startPos, short endPos, funcPtr ptr);
	funcToken(short startPos, short endPos, generalFuncPtr ptr);
	funcToken();
private:
		bool _is_general;
};
}
#endif // TOKENS_H_INCLUDED
