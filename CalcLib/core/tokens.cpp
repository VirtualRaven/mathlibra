#include "tokens.h"





	 bool  baseToken::hasNode()
	{
		return false;
	}
	 baseToken::~baseToken(){}

    baseToken::baseToken() : startPos(0), endPos(0), type(tokenType::UNKNOWN){}




	tree::nodeDataInterface * operatorToken::node()
	{
		switch (operType)
		{
		case operators::ASSIGN:
		{
					   return new mathNode::mathExpressionNode_opr(assign);
		}
			break;
		case operators::MATH:
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
	operatorToken::operatorToken(operators::operPtr opr, char symbol, short wheight)
	 :baseToken(),
	 operators::interpreter_operator( opr, symbol,wheight)
	{
		this->type = tree::OPERATOR;
	}
	operatorToken::operatorToken(operators::assigmentPtr assigne, char symbol, short wheight)
	 :baseToken(), operators::interpreter_operator(assign,symbol,wheight)
	{
		this->type = tree::OPERATOR;
	}
	operatorToken::operatorToken(operators::interpreter_operator opr)
	:baseToken(), operators::interpreter_operator(opr)
	{
		  this->type = tree::OPERATOR;
	}

	operatorToken::operatorToken()
    :baseToken(),operators::interpreter_operator()
    {
		this->type = tree::OPERATOR;
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
	tree::nodeDataInterface* parenthesesToken::node()
	{
		return nullptr;
	}
	parenthesesToken::parenthesesToken(short startPos, short endPos)
	: baseToken(),
    opposit(0)
	{
		this->endPos = endPos;
		this->startPos = startPos;
		this->type = tree::PARENTHESES;
	}
	parenthesesToken::parenthesesToken()
    : baseToken(),
    opposit(0)
	{
		this->endPos = 0;
		this->startPos = 0;
		this->type = tree::PARENTHESES;
		this->opposit =0;
	}


	tree::nodeDataInterface* valueToken::node()
	{
		return new mathNode::mathExpressionNode_val(value);
	}
	bool  valueToken::hasNode()
	{
		return true;
	}
	valueToken::valueToken(short startPos, short endPos)
	:baseToken(),
	value(0)
	{

		this->endPos = endPos;
		this->startPos = startPos;
		this->type = tree::VALUE;
	}

	valueToken::valueToken()
	:baseToken(),
	value(0)
	{
		this->type = tree::VALUE;
	}





	tree::nodeDataInterface* variableToken::node()
	{
		if (mem == nullptr)
		{
			throw memoryOops("Tried to create node variable without assigning memoryblock");
		}
		return new mathNode::mathExpressionNode_variable(variableName,mem,_stack);
	}
	bool  variableToken::hasNode()
	{
		return true;
	}
	variableToken::variableToken(short startPos, short endPos,memory* mem)
	:baseToken(),
	variableName(""),
	mem(mem),
	_stack(true)
	{
		this->endPos = endPos;
		this->startPos = startPos;
		this->type = tree::VARIABLE;
	}

	variableToken::variableToken()
	:baseToken(),
	variableName(""),
	mem(nullptr),
	_stack(true)
	{
		this->type = tree::VARIABLE;
	}
    variableToken::variableToken(const variableToken& val)
    :variableName(val.variableName),
    mem(val.mem),
	_stack(val._stack)
    {
        this->endPos = val.endPos;
        this->startPos = val.startPos;
        this->type = val.type;
    }
   variableToken variableToken::operator=(const variableToken& val)
    {
        this->endPos = val.endPos;
        this->mem = val.mem;
        this->startPos = val.startPos;
        this->type = val.type;
        this->variableName = val.variableName;
        return *this;
    }


   tree::nodeDataInterface* funcToken::node()
	{
		if (ptr == nullptr)
		{
			throw memoryOops("Function node missing pointer to actual function");
		}
		if (_is_general)
		{
			return new mathNode::mathExpressionNode_func_tree(gptr);
		}
		else
		{
			return new mathNode::mathExpressionNode_func(ptr);
		}
		
	}
	bool  funcToken::hasNode()
	{
		return true;
	}
	funcToken::funcToken(short startPos, short endPos, funcPtr ptr)
	:ptr(ptr),
	baseWheight(4),
	_is_general(false)
	{
		this->endPos = endPos;
		this->startPos = startPos;
		this->type = tree::FUNCTION;
	}
	funcToken::funcToken(short startPos, short endPos, generalFuncPtr ptr)
		:gptr(ptr),
		baseWheight(4),
		_is_general(true)
	{
		this->endPos = endPos;
		this->startPos = startPos;
		this->type = tree::FUNCTION;
	}
	funcToken::funcToken()
	:ptr(nullptr),
	baseWheight(4)
	{
		this->endPos = 0;
		this->startPos = 0;
		this->type = tree::FUNCTION;
	}

