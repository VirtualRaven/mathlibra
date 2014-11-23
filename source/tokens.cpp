#include "tokens.h"




	 mathNode::mathExpressionNode* baseToken::node()
	{
		return  new mathNode::mathExpressionNode();
	}
	 bool  baseToken::hasNode()
	{
		return false;
	}
	 baseToken::~baseToken()
	{

	}




	mathNode::mathExpressionNode * operatorToken::node()
	{
		switch (operType)
		{
		case ASSIGN:
		{
					   return new mathNode::mathExpressionNode_opr(assign);
		}
			break;
		case MATH:
		{
			return new mathNode::mathExpressionNode_opr(oper);
		}
			break;
		default:
		{
				   return nullptr;
		}
			break;
		}

	}
	bool  operatorToken::hasNode()
	{
		return true;
	}
	operatorToken::operatorToken(operPtr opr, char symbol, short wheight)
	{
		baseWheight = wheight;
		operChar = symbol;
		oper = opr;
		assign = nullptr;
		operType = MATH;
		this->endPos = 0;
		this->startPos = 0;
		this->type = OPERATOR;
	}
	operatorToken::operatorToken(assigmentPtr assign, char symbol, short wheight)
	{
		baseWheight = wheight;
		operChar = symbol;
		oper = nullptr;
		operType = ASSIGN;
		this->assign = assign;
		this->endPos = 0;
		this->startPos = 0;
		this->type = OPERATOR;
	}
	operatorToken::operatorToken()
	{
		baseWheight = 0;
		operChar = 0;
		oper = nullptr;
		assign = nullptr;
		operType = NOT_SET;
		this->endPos = 0;
		this->startPos = 0;
		this->type = OPERATOR;
	}

	bool parenthesesToken::isOppening()
	{
		if (startPos < opposit)
		{
			return true;
		}
		else if (startPos == opposit)
		{
			std::cerr << "-[WARNING: Using isOppening() on uninitilized paratheses ]\n";
			return false;
		}
		else return false;
	}
	bool parenthesesToken::hasNode()
	{
		return false;
	}
	parenthesesToken::parenthesesToken(short startPos, short endPos)
	{
		this->endPos = endPos;
		this->startPos = startPos;
		this->type = PARENTHESES;
		this->opposit =0;

	}
	parenthesesToken::parenthesesToken()
	{
		this->endPos = 0;
		this->startPos = 0;
		this->type = PARENTHESES;
		this->opposit =0;
	}


	mathNode::mathExpressionNode* valueToken::node()
	{
		return new mathNode::mathExpressionNode_val(value);
	}
	bool  valueToken::hasNode()
	{
		return true;
	}
	valueToken::valueToken(short startPos, short endPos)
	{
		this->endPos = endPos;
		this->startPos = startPos;
		this->type = VALUE;
	}

	valueToken::valueToken()
	{
		this->endPos = 0;
		this->startPos = 0;
		this->type = VALUE;
	}





	mathNode::mathExpressionNode* variableToken::node()
	{
		if (mem == nullptr)
		{
			throw memoryOops("Tried to create node variable without assigning memoryblock");
		}
		return new mathNode::mathExpressionNode_variable(variableName,mem);
	}
	bool  variableToken::hasNode()
	{
		return true;
	}
	variableToken::variableToken(short startPos, short endPos,memory* mem)
	{
		this->mem = mem;
		this->endPos = endPos;
		this->startPos = startPos;
		this->type =VARIABLE;
	}

	variableToken::variableToken()
	{
		this->mem = nullptr;
		this->endPos = 0;
		this->startPos = 0;
		this->type = VARIABLE;
	}



	mathNode::mathExpressionNode* funcToken::node()
	{
		if (ptr == nullptr)
		{
			throw memoryOops("Function node missing pointer to actual function");
		}
		return new mathNode::mathExpressionNode_func(ptr);
	}
	bool  funcToken::hasNode()
	{
		return true;
	}
	funcToken::funcToken(short startPos, short endPos, funcPtr ptr)
	{
		this->ptr = ptr;
		this->endPos = endPos;
		this->startPos = startPos;
		this->type = FUNCTION;
		this->baseWheight = 4;
	}

	funcToken::funcToken()
	{
		this->ptr = nullptr;
		this->endPos = 0;
		this->startPos = 0;
		this->type = FUNCTION;
		this->baseWheight = 4;
	}

