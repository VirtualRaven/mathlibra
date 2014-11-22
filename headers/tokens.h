#include "mathNode.h"





struct baseToken
{
	short startPos;
	short endPos;
	tokenType type;
	virtual mathNode::mathExpressionNode* node()
	{
		return  new mathNode::mathExpressionNode();
	}
	virtual bool  hasNode()
	{
		return false;
	}
	virtual ~baseToken()
	{

	}

};

template<typename mathType>
struct operatorToken : public baseToken
{
	typedef mathType (*operPtr)(mathType,mathType);
	typedef mathType (*assigmentPtr)(variableToken<mathType>, mathType);
	operPtr oper;
	assigmentPtr assign;
	char operChar;
	short baseWheight;
	enum operationType{NOT_SET,ASSIGN,MATH};
	operationType operType;
	mathNode::mathExpressionNode * node()
	{
		switch (operType)
		{
		case ASSIGN:
		{
					   return new mathNode::mathExpressionNode_opr<mathType>(assign);
		}
			break;
		case MATH:
		{
			return new mathNode::mathExpressionNode_opr<mathType>(oper);
		}
			break;
		default:
		{
				   return nullptr;
		}
			break;
		}
		
	}
	bool  hasNode()
	{
		return true;
	}
	operatorToken(operPtr opr, char symbol, short wheight)
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
	operatorToken(assigmentPtr assign, char symbol, short wheight)
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
	operatorToken()
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
};
template<typename mathType>
struct parenthesesToken : public baseToken
{

	short opposit;
	bool isOppening()
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
	bool  hasNode()
	{
		return false;
	}
	parenthesesToken(short startPos, short endPos)
	{
		this->endPos = endPos;
		this->startPos = startPos;
		this->type = PARENTHESES;
		this->opposit =0;

	}
	parenthesesToken()
	{
		this->endPos = 0;
		this->startPos = 0;
		this->type = PARENTHESES;
		this->opposit =0;
	}

};
template<typename mathType>
struct valueToken : public baseToken
{

	double value;
	mathNode::mathExpressionNode* node()
	{
		return new mathNode::mathExpressionNode_val<mathType>(value);
	}
	bool  hasNode()
	{
		return true;
	}
	valueToken(short startPos, short endPos)
	{
		this->endPos = endPos;
		this->startPos = startPos;
		this->type = VALUE;
	}

	valueToken()
	{
		this->endPos = 0;
		this->startPos = 0;
		this->type = VALUE;
	}

};

template<typename mathType>
struct variableToken : public baseToken
{

	std::string variableName;
	memory<mathType> *mem;
	mathNode::mathExpressionNode* node()
	{
		if (mem == nullptr)
		{
			throw memoryOops("Tried to create node variable without assigning memoryblock");
		}
		return new mathNode::mathExpressionNode_variable<mathType>(variableName,mem);
	}
	bool  hasNode()
	{
		return true;
	}
	variableToken(short startPos, short endPos,memory<mathType>* mem)
	{
		this->mem = mem;
		this->endPos = endPos;
		this->startPos = startPos;
		this->type =VARIABLE;
	}

	variableToken()
	{
		this->mem = nullptr;
		this->endPos = 0;
		this->startPos = 0;
		this->type = VARIABLE;
	}

};

template<typename mathType>
struct funcToken : public baseToken
{
	typedef mathType(*funcPtr)(mathType);
	funcPtr ptr;
	
	mathNode::mathExpressionNode* node()
	{
		if (ptr == nullptr)
		{
			throw memoryOops("Function node missing pointer to actual function");
		}
		return new mathNode::mathExpressionNode_func<mathType>(ptr);
	}
	bool  hasNode()
	{
		return true;
	}
	funcToken(short startPos, short endPos, funcPtr ptr)
	{
		this->ptr = ptr;
		this->endPos = endPos;
		this->startPos = startPos;
		this->type = FUNCTION;
	}

	funcToken()
	{
		this->ptr = nullptr;
		this->endPos = 0;
		this->startPos = 0;
		this->type = FUNCTION;
	}

};
