/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "exception_helper.h"
#include "lexical_internal.h"
#include <cstring>
#include <cstdlib>
#include "ptr_protect.h"
#include "type_helper.h"
using namespace __internal;

template<EXCEPTION T> void lexicalOops()
{
	__mathlibra__raise<T,INTERPRETER>();
}

//Checks that in an given context if the character '-' should be intepreted as
//an binary subtraction operator or an unary negation operator. 
bool isUnary(const char* str,  size_t i, tvec tokens)
{
	if (!tokens.empty())
	{
		token::baseToken * token = tokens.back();
		if (token->type == tree::PARENTHESES)
		{
			return  (i != 0) ? (str[i-1] == '(' || str[i-1]== '[' ) : false ;
                }
		else if (token->type == tree::OPERATOR)
		{
		  return true;
		}
			else return false;
	}
        else return true;
}

std::string extract_alpha(const char* str,size_t offset, size_t len)
{
	size_t end = offset;
	while( end < len && isalpha(str[end]))
	{
		end++;
	};
	return std::string(str,offset,end-offset);
}

state lexical(const char * expr,
	      size_t expr_len,
	      memory::memory* mem, 
	      math_func::function_interface* func,
	      operators::operators_interface* opr )
{
	tvec tokens;
	tokens.reserve(expr_len);
	
	pstack paran;
	std::stack<bool> extraParan;
	size_t oper_w_extra=0;
	size_t oper_w_lowset=99999999;
	size_t i=0;
	size_t start=0;
	i_state s = { &oper_w_extra, &i,&oper_w_lowset, &expr_len,&start};
	
	for (i=0; i< expr_len; i++)
	{
		if(expr[i] == '(')
		{
			if(i>0 && (expr[i-1] ==  ')' || expr[i-1]==']') )
			{
				pushMulti(tokens,opr,s);
			}
			parse_parantheses1(tokens,s,paran,opr);
		}		
		else if(expr[i] == ')')
		{
			parse_parantheses2(tokens,s,paran);
		}
                else if(expr[i]=='[')
                {
			extraParan.push(true);
                        if(func->isloaded("matc").loaded)
                        {	
		        	if (tokens.size() > 0 && (tokens.back()->type == tree::VALUE || (expr[i-1]==')' || expr[i-1]==']') ))
		        	{
		        		pushMulti(tokens,opr,s);
		        	}
			        parse_func("matc",tokens,s,func);
				parse_parantheses1(tokens,s,paran,opr);
				if(func->isloaded("mat").loaded){
			        	parse_func("mat",tokens,s,func);
				}
                        }      
			else
			{
				lexicalOops<MATRICE_SYNTAX_DISABLED>();
			}
			parse_parantheses1(tokens,s,paran,opr);
                }
                else if(expr[i]==']')
                {
			if(extraParan.size()>0)
			{
				extraParan.pop();	
			}
			else
			{
				lexicalOops<SQUARE_BRACKET_MISSMATCH>();
			}
			parse_parantheses2(tokens,s,paran);
			parse_parantheses2(tokens,s,paran);
                }
                else if(expr[i]=='|')
                {
			if(i==0 || (i>0 && expr[i-1]=='['))
			{
				lexicalOops<MATRICE_NEW_LINE_UNALLOWED>();
			}	
			if(extraParan.size() > 0)
			{
					parse_parantheses2(tokens,s,paran);
			}
			else
			{
				lexicalOops<MATRICE_NEW_LINE_SYMBOL>();	
			}
				opr->inArray(',');
				parse_opr(tokens,s,opr);
			func->isloaded("mat");
			parse_func("mat",tokens,s,func);
			parse_parantheses1(tokens,s,paran,opr);
                }
		else if (isdigit(expr[i]) || expr[i]== '.')
		{
			parse_number(expr,tokens,s,opr);	
		}
		else if(expr[i] == '-'  && 
			isUnary(expr,i,tokens) &&
			i+1<expr_len &&
			(isdigit(expr[i+1]) || expr[i+1] == '.'))
		{
			parse_number(expr,tokens,s,opr);
		}
		else if (expr[i] == '"')
		{
			parse_string(expr, tokens, s);
		}
		else if (opr!= nullptr && opr->inArray(expr[i]))
		{	
			//Following test compensates for implicit -1 situations like -x or 2*-(2+x) which 
			//will be extended to -1*x and 2*-1*(2+x) where the multiplication sign
			//between -1 and the other term has an higher than usal weight
			if ( expr[*(s.i)] == '-')
			{
				tree::tokenType test_type;
				if (tokens.size() > 0)
				{
					test_type = tokens.back()->type;
				}
				else
				{
					test_type = tree::UNKNOWN;
				}
		
				if ((  *(s.i) - 1 > 0 && (test_type == tree::OPERATOR || ( expr[*(s.i) - 1] == '(' || expr[*(s.i)-1] == '[' ))) || *(s.i) == 0)  
				{
					if (opr->inArray('*'))
					{
						pushValue(tokens,s,-1);	
						pushMulti(tokens,opr,s);
						continue;
					}		
				}
			}
			parse_opr(tokens,s,opr);
		}
		else if( isalpha(expr[i]))
		{
			if (tokens.size() > 0 && (tokens.back()->type == tree::VALUE || (expr[i-1]==')' || expr[i-1]==']') ))
			{
				pushMulti(tokens,opr,s);
			}

			auto str = extract_alpha(expr,i,expr_len);
			i+=str.size()-1;	
			if (func != nullptr && func->isloaded(str).loaded)
			{
				parse_func(str,tokens,s,func);
			}
			else if (mem != nullptr)
			{
				parse_var(str,tokens,s,func,mem);
			}
			else
			{
				lexicalOops<VARIABLES_DISABLED_BUT_USED>();
			}
		}
		else 
		{
			lexicalOops<UNKNOWN_CHAR_STR>();
		}
	}
#ifndef NON_STRICT_PARANTHESES
	if(expr[expr_len-1]=='|')
	{
		lexicalOops<MATRICE_NEW_LINE_NOT_LAST>();
	}	
	else if(extraParan.size() != 0)
	{
		lexicalOops<SYNTAX_UNMATCHED_CLOSING_SQUARE_PARANTHESES>();
	}
	if(!paran.empty())
	{
		lexicalOops<SYNTAX_UNMATCHED_OPENING_PARANTHESES>();
	}
	else
	{
		tokens.shrink_to_fit();
	}
#else
	tokens.shrink_to_fit();
#endif //SRICT_PARATHESES
	return  {*(s.start) ,tokens};	
}

namespace __internal
{
//****************************************************
//* Util
//***************************************************


void pushMulti(tvec& t,operators::operators_interface* opr, i_state& s)
{
	if (opr->inArray('*'))
	{
		ptr_protect<token::operatorToken*,false> tmp(new token::operatorToken(opr->getCurrent()));
		tmp->startPos=*s.i;
		tmp->endPos=*s.i;
		tmp->baseWheight +=*s.operWheight;
		updateStartPoint(t,s,tmp->baseWheight);	
		t.push_back(tmp.ptr());
		tmp.release();

	}
}

void pushValue(tvec& tokens ,i_state& s, double value)
{
	ptr_protect<token::valueToken*, false> tmp_val(new token::valueToken(*(s.i),*(s.i)));
	tmp_val->value = interface::type_ptr(make_type(value));
	tokens.push_back(tmp_val.ptr());
	tmp_val.release();
}

void updateStartPoint(tvec& t, i_state& s, size_t w)
{
	if(w <= *s.lowestWheight)
	{
		*s.lowestWheight = w;
		*(s.start)=t.size();
	}
}
//*****************************************************
//* Parantheses parsers	
//*
//*****************************************************

void parse_parantheses1(tvec& tokens,
			i_state& s,
			pstack& stack,
			operators::operators_interface* opr)
{
	auto tmp_i=*(s.i);
	auto type = (tokens.size() > 0) ? tokens.back()->type : tree::UNKNOWN ;
	if( type == tree::VALUE || type ==tree::VARIABLE)
	{
		pushMulti(tokens,opr,s);
		tmp_i++;
	}
	tokens.push_back(new token::parenthesesToken(tmp_i,tmp_i));
	stack.push((token::parenthesesToken*) tokens.back());
	(*s.operWheight)+=5;
	return;
	
}


void parse_parantheses2(tvec& tokens ,i_state& s, pstack& p)
{
	if(p.empty())
	{
		lexicalOops<SYNTAX_UNMATCHED_CLOSING_PARANTHESES>();
		
	}
	else
	{
		token::parenthesesToken* tmp = p.top();
		p.pop();
		tmp->opposit=*(s.i);
		token::parenthesesToken * tmp2 = new token::parenthesesToken(*(s.i), *(s.i));
		tmp2->opposit = tmp->startPos;
		tokens.push_back(tmp2);
		*(s.operWheight)-=5;
		return;

	}
}
//**************************************************
//* string parser
//**************************************************

void parse_string(const char * expr, tvec& tokens, i_state& s)
{
	
	for (size_t i = *s.i+1; i < *s.lenght; i++)
	{
		if (expr[i] == '"')
		{
			ptr_protect<token::valueToken*, false> tmp(new token::valueToken(*(s.i), 0));
			tmp->startPos = *s.i;
			tmp->endPos = i;
	
			if (*s.i - i == 1)
			{
				tmp->value= interface::type_ptr(make_type(""));
			}
			else
			{
				tmp->value= interface::type_ptr(make_type(std::string(expr + (*s.i) + 1, i - (*s.i) - 1)));
			}
			
			tokens.push_back(tmp.ptr());
			tmp.release(); //Release ownership of pointer
			*(s.i) = i;
			return;
		}
	}
	lexicalOops<SYNTAX_EXPECTED_END_OF_STRING>();
}

//**************************************************
//*	number parser
//*
//**************************************************

void parse_number(const char * expr, tvec& tokens, i_state& s,operators::operators_interface* opr)  
{
	if(tokens.size() > 0) 
	{
		auto tmp=tokens.back();
		if(tmp->type == tree::VARIABLE)
		{
			pushMulti(tokens,opr,s);
		}
		else if(expr[*(s.i)-1]==')' || expr[*(s.i)-1]==']')
		{
			pushMulti(tokens,opr,s);
		}
	}
	ptr_protect<token::valueToken*, false> tmp(new token::valueToken(*(s.i), 0));
	short valueLength=0;
	for(unsigned int i2 =*(s.i)+1; i2 < *(s.lenght) ; i2++)
	{
		if( !isdigit(expr[i2]) && 
			expr[i2] !='.' && 
			expr[i2] != 'e' && 
			expr[i2] != '-' )
		{
			break;
		}
		else if(expr[i2] == 'e')
		{
			if(i2+1 < *(s.lenght) &&
				( !isdigit(expr[i2+1]) && expr[i2+1] !='-') 
				)
			{
				lexicalOops<SYNTAX_EXP_AFTER_E>();
			}
			else if(i2+1 >=*(s.lenght))
			{
				lexicalOops<SYNTAX_EXP_AFTER_E>();
			}
	
		}
		else if(expr[i2] == '-')
		{
			//If expression is > instead of >= it will fail for expressions as 0-x^2
			if(i2-1 < *(s.lenght)  && expr[i2-1] != 'e') 
				break;

		}
			valueLength++;
	}
	tmp->endPos = *(s.i)+valueLength;
	size_t tmp_str_length = tmp->endPos+1 - tmp->startPos;
	//Create a temporary string from which we can convert the string to a double
	char* tmp_str = nullptr;
        tmp_str = new char[tmp_str_length+1];
	memcpy(tmp_str, &expr[*(s.i)], (tmp_str_length*sizeof(char)) );
	tmp_str[tmp_str_length] = '\0';
	tmp->value =  interface::type_ptr(make_type(atof(tmp_str)));
	delete[] tmp_str;
	tokens.push_back(tmp.ptr());
	tmp.release(); //Release ownership of pointer
	*(s.i)+=valueLength;
	return;

}

//**********************************************
//*	Operator parser
//*
//**********************************************
void parse_opr(tvec& tokens ,i_state& s,operators::operators_interface* opr)
{

	ptr_protect<token::operatorToken*, false> tmp(new token::operatorToken(opr->getCurrent()));
	tmp->startPos=*(s.i);
	tmp->endPos=*(s.i);
	tmp->baseWheight +=*(s.operWheight);
	updateStartPoint(tokens,s,tmp->baseWheight);
	if (tmp->operChar == '=')
	{
		if (tokens.size() ==0 || tokens.back()->type != tokenType::VARIABLE)
		{
			lexicalOops<SYNTAX_ASSIGMENT_NEEDS_VAR>();
		}
		else
		{
			//token is going to be used in an assigment 
			//and should thus not be pushed to the stack
			static_cast<token::variableToken*>(tokens.back())->_stack = false;
		}
	}
	tokens.push_back(tmp.ptr());
	tmp.release();
	return;
}

//*****************************************
//*	Function Parser
//*
//*****************************************

void parse_func(std::string str,
		tvec& tokens,
		i_state& s,
		math_func::function_interface* func)
{
	token::funcToken * tmp_ptr;
	math_func::func_type type = func->type();
	auto startPos=*(s.i);
	auto endPos = *(s.i)+str.size();
	if (type == math_func::func_type::GENERAL )
	{
		tmp_ptr = new token::funcToken(startPos, endPos, (token::funcToken::generalFuncPtr)func->get(str));
	}
        else
        {
            tmp_ptr = new token::funcToken(startPos,endPos,func->getFunctor(str));
        }

	ptr_protect<token::funcToken *, false> tmp(tmp_ptr);
	
	tmp->baseWheight += *(s.operWheight);
	updateStartPoint(tokens,s,tmp->baseWheight);	
	tokens.push_back(tmp.ptr());
	tmp.release();
}

void parse_var(std::string str,
		tvec& tokens,
		i_state& s, 
		math_func::function_interface* func,
		memory::memory* mem)
{

	token::variableToken * tmp = new token::variableToken(*(s.i),*(s.i)+str.size() , mem,func);
	tmp->variableName = str;
	tokens.push_back(tmp);
}

}

