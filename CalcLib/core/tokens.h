#ifndef TOKENS_H_INCLUDED
#define TOKENS_H_INCLUDED

#include "mathNode.h"
#include "tree.h"
#include "modules/operators.h"
#include "main.h"
using mathNode::number_type;
using tree::tokenType;

struct baseToken
{
	short startPos;
	short endPos;
	tokenType type;

	virtual tree::nodeDataInterface* node()=0;
	virtual bool  hasNode();
	baseToken();
	virtual ~baseToken();

};


struct operatorToken : public baseToken, operators::interpreter_operator
{
	tree::nodeDataInterface * node();
	bool  hasNode();
	operatorToken(operators::operPtr opr, char symbol, short wheight);
	operatorToken(operators::assigmentPtr assign, char symbol, short wheight);
	operatorToken(operators::interpreter_operator opr);
	operatorToken();
};

struct parenthesesToken : public baseToken
{

	short opposit;

	bool isOppening();
	bool  hasNode();
	tree::nodeDataInterface * node();
	parenthesesToken(short startPos, short endPos);
	parenthesesToken();

};

struct valueToken : public baseToken
{
	double value;

	tree::nodeDataInterface* node();
	bool  hasNode();
	valueToken(short startPos, short endPos);
	valueToken();
};


struct variableToken : public baseToken
{

	std::string variableName;
	memory *mem;
	bool _stack;
	tree::nodeDataInterface* node();
	bool  hasNode();
	variableToken(short startPos, short endPos,memory* mem);
	variableToken();
    variableToken(const variableToken&);
    variableToken operator=(const variableToken&);


};


struct funcToken : public baseToken
{

	typedef number_type(*funcPtr)(number_type);
	typedef number_type(*generalFuncPtr)(tree::node_base*);
	union
	{
		funcPtr ptr;
		generalFuncPtr gptr;
	};
	short baseWheight;
	tree::nodeDataInterface* node();
	bool  hasNode();
	funcToken(short startPos, short endPos, funcPtr ptr);
	funcToken(short startPos, short endPos, generalFuncPtr ptr);
	funcToken();
private:
		bool _is_general;
};
#endif // TOKENS_H_INCLUDED
