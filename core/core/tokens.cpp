/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "tokens.h"
#include "exception_helper.h"
#include "core/type_helper.h"

	template<EXCEPTION T> void tokenOops()
	{
		__mathlibra__raise<T,TOKEN>();
	}

	 bool  token::baseToken::hasNode()
	{
		return false;
	}
	 token::baseToken::~baseToken(){}

	 token::baseToken::baseToken() :  type(tokenType::UNKNOWN){}




	 tree::nodeDataInterface * token::operatorToken::node()
	{
		return new mathNode::mathExpressionNode_opr(this->ptr);

	}
	 bool  token::operatorToken::hasNode()
	{
		return true;
	}
	 token::operatorToken::operatorToken(operators::generic_oper_ptr opr, char symbol, short wheight)
	 :baseToken(),
	 operators::interpreter_operator( opr, symbol,wheight)
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
		return opening;
	}
	 bool token::parenthesesToken::hasNode()
	{
		return false;
	}
	 tree::nodeDataInterface* token::parenthesesToken::node()
	{
		return nullptr;
	}
	 token::parenthesesToken::parenthesesToken(bool open)
	: baseToken(),opening(open)
	{
		this->type = tree::PARENTHESES;
	}


	 tree::nodeDataInterface* token::valueToken::node()
	{
		return new mathNode::mathExpressionNode_val(this->value->copy());
	}
	 bool token::valueToken::hasNode()
	{
		return true;
	}
	 token::valueToken::valueToken()
	:baseToken(),
	value(nullptr)
	{

		this->type = tree::VALUE;
	}






	 tree::nodeDataInterface* token::variableToken::node()
	{
		if (mem == nullptr)
		{
			tokenOops<TOKEN_NO_MEM_MODULE>();
			return nullptr;
		}
		return new mathNode::mathExpressionNode_variable(variableName,mem,_stack,func);
	}
	 bool  token::variableToken::hasNode()
	{
		return true;
	}
	 token::variableToken::variableToken(memory::memory* mem, math_func::function_interface* func)
	:baseToken(),
        func(func),
	variableName(""),
	mem(mem),
	_stack(true)
	{
		this->type = tree::VARIABLE;
	}

	 token::variableToken::variableToken()
	:baseToken(),
        func(nullptr),
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
        this->type = val.type;
    }
	 token::variableToken token::variableToken::operator=(const token::variableToken& val)
    {
        this->mem = val.mem;
        this->type = val.type;
        this->variableName = val.variableName;
        return *this;
    }

using token::ptr_type_enum;
        
	 tree::nodeDataInterface* token::funcToken::node()
	{
                
		if (gptr == nullptr)
		{
			tokenOops<TOKEN_FUNCTION_NULL_POINTER>();
			return nullptr;
		}
		else if (this->ptr_type == ptr_type_enum::gen)
		{
			return new mathNode::mathExpressionNode_func_tree(gptr);
		}
                else 
                {
                    return new mathNode::mathExpressionNode_func_user(uptr);
                }
		
	}
	 bool  token::funcToken::hasNode()
	{
		return true;
	}
	 	 token::funcToken::funcToken(generalFuncPtr ptr)
	:gptr(ptr),
	baseWheight(4),
	ptr_type(ptr_type_enum::gen)
	{
		this->type = tree::FUNCTION;
	}
        token::funcToken::funcToken(usr_ptr ptr)
        : uptr(ptr),
        baseWheight(4),
        ptr_type(ptr_type_enum::usr)
        {
            this->type=tree::FUNCTION;
        }

	 token::funcToken::funcToken()
	:gptr(nullptr),
	baseWheight(4)
	{
		this->type = tree::FUNCTION_TREE;
	}

