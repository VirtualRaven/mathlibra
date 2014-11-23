#include "mathNode.h"


using mathNode::number_type;


struct baseToken
{
	short startPos;
	short endPos;
	tokenType type;

	virtual mathNode::mathExpressionNode* node();
	virtual bool  hasNode();
	virtual ~baseToken();

};


struct operatorToken : public baseToken
{
	typedef number_type(*operPtr)(number_type,number_type);
	typedef number_type(*assigmentPtr)(variableToken, number_type);
	operPtr oper;
	assigmentPtr assign;
	char operChar;
	short baseWheight;
	enum operationType{NOT_SET,ASSIGN,MATH};
	operationType operType;

	mathNode::mathExpressionNode * node();
	bool  hasNode();
	operatorToken(operPtr opr, char symbol, short wheight);
	operatorToken(assigmentPtr assign, char symbol, short wheight);
	operatorToken();
};

struct parenthesesToken : public baseToken
{

	short opposit;

	bool isOppening();
	bool  hasNode();
	parenthesesToken(short startPos, short endPos);
	parenthesesToken();

};

struct valueToken : public baseToken
{

	double value;

	mathNode::mathExpressionNode* node();
	bool  hasNode();
	valueToken(short startPos, short endPos);
	valueToken();
};


struct variableToken : public baseToken
{

	std::string variableName;
	memory *mem;

	mathNode::mathExpressionNode* node();
	bool  hasNode();
	variableToken(short startPos, short endPos,memory* mem);
	variableToken();

};


struct funcToken : public baseToken
{
	typedef number_type(*funcPtr)(number_type);
	funcPtr ptr;

	mathNode::mathExpressionNode* node();
	bool  hasNode();
	funcToken(short startPos, short endPos, funcPtr ptr);
	funcToken();
};
