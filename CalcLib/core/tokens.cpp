#include "tokens.h"





	 bool  token::baseToken::hasNode()
	{
		return false;
	}
	 token::baseToken::~baseToken(){}

	 token::baseToken::baseToken() : startPos(0), endPos(0), type(tokenType::UNKNOWN){}




	 tree::nodeDataInterface * token::operatorToken::node()
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
	 bool  token::operatorToken::hasNode()
	{
		return true;
	}
	 token::operatorToken::operatorToken(operators::operPtr opr, char symbol, short wheight)
	 :baseToken(),
	 operators::interpreter_operator( opr, symbol,wheight)
	{
		this->type = tree::OPERATOR;
	}
	 token::operatorToken::operatorToken(operators::assigmentPtr assigne, char symbol, short wheight)
	 :baseToken(), operators::interpreter_operator(assign,symbol,wheight)
	{
		this->type = tree::OPERATOR;
	}
	 token::operatorToken::operatorToken(operators::interpreter_operator opr)
	:baseToken(), operators::interpreter_operator(opr)
	{
		  this->type = tree::OPERATOR;
	}

	 token::operatorToken::operatorToken()
    :baseToken(),operators::interpreter_operator()
    {
		this->type = tree::OPERATOR;
    }


	 bool token::parenthesesToken::isOppening()
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
	 bool token::parenthesesToken::hasNode()
	{
		return false;
	}
	 tree::nodeDataInterface* token::parenthesesToken::node()
	{
		return nullptr;
	}
	 token::parenthesesToken::parenthesesToken(short startPos, short endPos)
	: baseToken(),
    opposit(0)
	{
		this->endPos = endPos;
		this->startPos = startPos;
		this->type = tree::PARENTHESES;
	}
	 token::parenthesesToken::parenthesesToken()
    : baseToken(),
    opposit(0)
	{
		this->endPos = 0;
		this->startPos = 0;
		this->type = tree::PARENTHESES;
		this->opposit =0;
	}


	 tree::nodeDataInterface* token::valueToken::node()
	{
		return new mathNode::mathExpressionNode_val(value);
	}
	 bool token::valueToken::hasNode()
	{
		return true;
	}
	 token::valueToken::valueToken(short startPos, short endPos)
	:baseToken(),
	value(0)
	{

		this->endPos = endPos;
		this->startPos = startPos;
		this->type = tree::VALUE;
	}

	 token::valueToken::valueToken()
	:baseToken(),
	value(0)
	{
		this->type = tree::VALUE;
	}





	 tree::nodeDataInterface* token::variableToken::node()
	{
		if (mem == nullptr)
		{
			throw memory::memoryOops("Tried to create node variable without assigning memoryblock");
		}
		return new mathNode::mathExpressionNode_variable(variableName,mem,_stack);
	}
	 bool  token::variableToken::hasNode()
	{
		return true;
	}
	 token::variableToken::variableToken(short startPos, short endPos, memory::memory* mem)
	:baseToken(),
	variableName(""),
	mem(mem),
	_stack(true)
	{
		this->endPos = endPos;
		this->startPos = startPos;
		this->type = tree::VARIABLE;
	}

	 token::variableToken::variableToken()
	:baseToken(),
	variableName(""),
	mem(nullptr),
	_stack(true)
	{
		this->type = tree::VARIABLE;
	}
	 token::variableToken::variableToken(const token::variableToken& val)
    :variableName(val.variableName),
    mem(val.mem),
	_stack(val._stack)
    {
        this->endPos = val.endPos;
        this->startPos = val.startPos;
        this->type = val.type;
    }
	 token::variableToken token::variableToken::operator=(const token::variableToken& val)
    {
        this->endPos = val.endPos;
        this->mem = val.mem;
        this->startPos = val.startPos;
        this->type = val.type;
        this->variableName = val.variableName;
        return *this;
    }


	 tree::nodeDataInterface* token::funcToken::node()
	{
		if (ptr == nullptr)
		{
			throw memory::memoryOops("Function node missing pointer to actual function");
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
	 bool  token::funcToken::hasNode()
	{
		return true;
	}
	 token::funcToken::funcToken(short startPos, short endPos, funcPtr ptr)
	:ptr(ptr),
	baseWheight(4),
	_is_general(false)
	{
		this->endPos = endPos;
		this->startPos = startPos;
		this->type = tree::FUNCTION;
	}
	 token::funcToken::funcToken(short startPos, short endPos, generalFuncPtr ptr)
		:gptr(ptr),
		baseWheight(4),
		_is_general(true)
	{
		this->endPos = endPos;
		this->startPos = startPos;
		this->type = tree::FUNCTION;
	}
	 token::funcToken::funcToken()
	:ptr(nullptr),
	baseWheight(4)
	{
		this->endPos = 0;
		this->startPos = 0;
		this->type = tree::FUNCTION;
	}

