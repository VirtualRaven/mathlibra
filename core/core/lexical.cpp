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

        void __pushItem(const token_list::type t,size_t w, __internal::i_state& i,bool prev,size_t offset )
        {
            size_t next=0;
            if(prev)
            {
                if(t==token_list::type::PAR && !i.pstack.empty() ){
                    next=i.pstack.top();
                    i.pstack.pop();
                }
                else if(t==token_list::type::MAT && !i.mstack.empty()){
                    next=i.mstack.top();
                    i.mstack.pop();
                }
                if(prev)
                    i.token_list.push_back(token_list::item(t,w,i.i, i.i+offset,next));
            }
            else
                i.token_list.push_back(token_list::item(t,w,i.i, i.i+offset));
        }

//Checks that in an given context if the character '-' should be intepreted as
//an binary subtraction operator or an unary negation operator. 
bool isUnary(size_t i, tvec tokens)
{
	if (!tokens.empty())
	{
		token::baseToken * token = tokens.back();
		if (token->type == tree::PARENTHESES)
		{
			return  (i != 0) ? ( static_cast<token::parenthesesToken*>(token)->isOppening() ) : false ;
                }
		else if (token->type == tree::OPERATOR)
		{
		  return true;
		}
			else return false;
	}
        else return true;
}

inline bool isTokenOpeningParantheses(token::baseToken* token, bool closing=false){
    return token->type == tree::PARENTHESES  && 
         static_cast<token::parenthesesToken*>(token)->isOppening()^closing;
}
inline bool isLastTokenOpeningParantheses(tvec token, bool closing=false){
    if(!token.empty())
        return isTokenOpeningParantheses(token.back(),closing);
    else
        return false;
}

inline bool isTokenClosingParantheses(token::baseToken* token){
    return isTokenOpeningParantheses(token,true);
}

inline bool isLastTokenClosingParantheses(tvec tokens){
    return isLastTokenOpeningParantheses(tokens,true);
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
	i_state s = { 0, 
            0,999999999, 
            expr_len,0,
            token_list::token_list(),
            std::stack<size_t>(),
            std::stack<size_t>(), 
            std::stack<size_t>() 
        };
	
	for (s.i=0; s.i< expr_len; s.i++)
	{
		if(expr[s.i] == '(')
		{
			if(isLastTokenClosingParantheses(tokens) )
			{
				pushMulti(tokens,opr,s);
			}
			parse_parantheses1(tokens,s,paran,opr);
                        pushItem(token_list::type::PAR,s.operWheight,s);
		}		
		else if(expr[s.i] == ')')
		{
			parse_parantheses2(tokens,s,paran);
                        pushItem2(token_list::type::PAR,s.operWheight,s);
		}
                else if(expr[s.i]=='[')
                {
                        pushItem(token_list::type::MAT,s.operWheight,s);
                        s.mstack.push(s.i);
			extraParan.push(true);
                        if(func->isloaded("matc").loaded)
                        {	
		        	if (tokens.size() > 0)
                                {
                                    auto to= tokens.back();
                                    if( to->type == tree::VALUE || 
                                            isTokenClosingParantheses(to)
                                    )
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
                else if(expr[s.i]==']')
                {
                        pushItem2(token_list::type::MAT,s.operWheight,s);
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
                else if(expr[s.i]=='|')
                {
			if(s.i==0 || (s.i>0 && expr[s.i-1]=='['))
				lexicalOops<MATRICE_NEW_LINE_UNALLOWED>();
			if(extraParan.size() > 0)
					parse_parantheses2(tokens,s,paran);
			else
				lexicalOops<MATRICE_NEW_LINE_SYMBOL>();	
			opr->inArray(',');
			parse_opr(tokens,s,opr);
			func->isloaded("mat");
			parse_func("mat",tokens,s,func);
			parse_parantheses1(tokens,s,paran,opr);
                }
		else if (isdigit(expr[s.i]) || expr[s.i]== '.')
		{
			parse_number(expr,tokens,s,opr);	
		}
		else if(expr[s.i] == '-'  && 
			isUnary(s.i,tokens) &&
			s.i+1<expr_len &&
			(isdigit(expr[s.i+1]) || expr[s.i+1] == '.'))
		{
			parse_number(expr,tokens,s,opr);
		}
		else if (expr[s.i] == '"')
		{
			parse_string(expr, tokens, s);
		}
		else if (opr!= nullptr && opr->inArray(expr[s.i]))
		{	
			//Following test compensates for implicit -1 situations like -x or 2*-(2+x) which 
			//will be extended to -1*x and 2*-1*(2+x) where the multiplication sign
			//between -1 and the other term has an higher than usal weight
			if ( expr[s.i] == '-')
			{
				tree::tokenType test_type;
				if (tokens.size() > 0)
					test_type = tokens.back()->type;
				else
					test_type = tree::UNKNOWN;
		
				if (  (s.i > 0 && (test_type == tree::OPERATOR || isLastTokenOpeningParantheses(tokens))) //if last token is a operator or opening par
                                        || s.i == 0)  //or if this is the first token
				{
					if (opr->inArray('*'))
					{
						pushValue(tokens,-1);	
						pushMulti(tokens,opr,s);
						continue;
					}		
				}
			}

			parse_opr(tokens,s,opr,true);
		}
		else if( isalpha(expr[s.i]))
		{
			if (tokens.size() > 0 )
			{
                            auto t = tokens.back()->type;
                            if( t== tree::VALUE || 
                                isLastTokenClosingParantheses(tokens)  || 
                                t==tree::VARIABLE
                                )
				pushMulti(tokens,opr,s);
			}

			auto str = extract_alpha(expr,s.i,expr_len);
			if (func != nullptr && func->isloaded(str).loaded)
			{
				parse_func(str,tokens,s,func,true);
			}
			else if (mem != nullptr)
			{
				parse_var(str,tokens,s,func,mem);
			}
			else
			{
				lexicalOops<VARIABLES_DISABLED_BUT_USED>();
			}
			s.i+=str.size()-1;	
		}
                else if(isspace(expr[s.i])){
                        //This code detects when a space should mark the end of function arguments
                        //for example sqrt sin 2 3 becomes sqrt(sin(2))3 =which in turn becomes sqrt(sin(2))*3
                        if( tokens.size() > 1){
                            //Last token shall not be a function, unfortunately, functions also pushes parantheses so we must 
                            //check the last token and the next to last token.
                            if(!  (tokens.back()->type == tree::PARENTHESES && (*(tokens.end()-2))->type ==tree::FUNCTION)) {
                                while(s.fstack.size() > 0 && s.fstack.top()== s.operWheight){
                                    s.operWheight-=5;
                                    s.fstack.pop();
                                    tokens.push_back(new token::parenthesesToken(false));
                                }
                            }
                        }
                }
		else 
		    lexicalOops<UNKNOWN_CHAR_STR>();
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
        return {s.start,tokens, s.token_list};
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
		tmp->baseWheight +=s.operWheight;
		updateStartPoint(t,s,tmp->baseWheight);	
		t.push_back(tmp.ptr());
		tmp.release();

	}
}

void pushValue(tvec& tokens , double value)
{
	ptr_protect<token::valueToken*, false> tmp_val(new token::valueToken());
	tmp_val->value = interface::type_ptr(make_type(value));
	tokens.push_back(tmp_val.ptr());
	tmp_val.release();
}

void updateStartPoint(tvec& t, i_state& s, size_t w)
{
	if(w <= s.lowestWheight)
	{
		s.lowestWheight = w;
		s.start=t.size();
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
        if(tokens.size()> 1)
            
            if(tokens.back()->type == tree::PARENTHESES && (*(tokens.end()-2))->type ==tree::FUNCTION) {
                if(stack.size()==0 ||  stack.top() != tokens.back())
                {
                    s.fstack.pop();
	            stack.push((token::parenthesesToken*) tokens.back() );
                    return;
                }
            }
	auto tmp_i=s.i;
	auto type = (tokens.size() > 0) ? tokens.back()->type : tree::UNKNOWN ;
	if( type == tree::VALUE || type ==tree::VARIABLE)
	{
		pushMulti(tokens,opr,s);
		tmp_i++;
	}
	tokens.push_back(new token::parenthesesToken(true));
	stack.push((token::parenthesesToken*) tokens.back() );
        s.operWheight+=5;
        s.pstack.push(s.i);
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
		p.pop();
		token::parenthesesToken * tmp2 = new token::parenthesesToken(false);
		tokens.push_back(tmp2);
	        s.operWheight-=5;
		return;

	}
}
//**************************************************
//* string parser
//**************************************************

void parse_string(const char * expr, tvec& tokens, i_state& s)
{
	
	for (size_t i = s.i+1; i < s.lenght; i++)
	{
		if (expr[i] == '"')
		{
			ptr_protect<token::valueToken*, false> tmp(new token::valueToken());
	
			if (s.i - i == 1)
				tmp->value= interface::type_ptr(make_type(""));
			else
				tmp->value= interface::type_ptr(make_type(std::string(expr+s.i+1, i-s.i-1)));
			
			tokens.push_back(tmp.ptr());
			tmp.release(); //Release ownership of pointer
                        pushItem3(token_list::type::STR,s.operWheight,s,i-s.i);
			s.i = i;
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
		if(tmp->type == tree::VARIABLE || tmp->type == tree::VALUE)
		{
			pushMulti(tokens,opr,s);
		}
		else if(isTokenClosingParantheses(tmp))
		{
			pushMulti(tokens,opr,s);
		}
	}
	ptr_protect<token::valueToken*, false> tmp(new token::valueToken());
	short valueLength=0;
	for(unsigned int i2 =s.i+1; i2 < s.lenght ; i2++)
	{
		if( !isdigit(expr[i2]) && 
			expr[i2] !='.' && 
			expr[i2] != 'e' && 
			expr[i2] != '-' )
			break;
		else if(expr[i2] == 'e')
		{
			if(i2+1 < s.lenght &&
				(!isdigit(expr[i2+1]) && expr[i2+1] !='-') 
			    )
			    lexicalOops<SYNTAX_EXP_AFTER_E>();
			else if(i2+1 >=s.lenght)
				lexicalOops<SYNTAX_EXP_AFTER_E>();
	
		}
		else if(expr[i2] == '-')
			//If expression is > instead of >= it will fail for expressions as 0-x^2
			if(i2-1 < s.lenght  && expr[i2-1] != 'e') 
				break;

			valueLength++;
	}
	size_t tmp_str_length = valueLength+1;
	//Create a temporary string from which we can convert the string to a double
	char* tmp_str = nullptr;
        tmp_str = new char[tmp_str_length+1];
	memcpy(tmp_str, &expr[s.i], (tmp_str_length*sizeof(char)) );
	tmp_str[tmp_str_length] = '\0';
	tmp->value =  interface::type_ptr(make_type(atof(tmp_str)));
	delete[] tmp_str;
	tokens.push_back(tmp.ptr());
	tmp.release(); //Release ownership of pointer
        pushItem3(token_list::type::VAL,s.operWheight,s,valueLength);
	s.i+=valueLength;
	return;

}

//**********************************************
//*	Operator parser
//*
//**********************************************
void parse_opr(tvec& tokens ,i_state& s,operators::operators_interface* opr, bool show)
{

	ptr_protect<token::operatorToken*, false> tmp(new token::operatorToken(opr->getCurrent()));
	tmp->baseWheight +=s.operWheight;
	updateStartPoint(tokens,s,tmp->baseWheight);
	if (tmp->operChar == '=')
	{
		if (tokens.size() ==0 || tokens.back()->type != tokenType::VARIABLE)
			lexicalOops<SYNTAX_ASSIGMENT_NEEDS_VAR>();
		else
		{
			//token is going to be used in an assigment 
			//and should thus not be pushed to the stack
			static_cast<token::variableToken*>(tokens.back())->_stack = false;
		}
	}
	tokens.push_back(tmp.ptr());
	tmp.release();
        if(show)
            pushItem(token_list::type::OPER,tmp->baseWheight,s);
	return;
}

//*****************************************
//*	Function Parser
//*
//*****************************************

void parse_func(std::string str,
		tvec& tokens,
		i_state& s,
		math_func::function_interface* func, bool show)
{
	token::funcToken * tmp_ptr;
	math_func::func_type type = func->type();
	if (type == math_func::func_type::GENERAL )
	{
		tmp_ptr = new token::funcToken((token::funcToken::generalFuncPtr)func->get(str));
	}
        else
        {
            tmp_ptr = new token::funcToken(func->getFunctor(str));
        }

	ptr_protect<token::funcToken *, false> tmp(tmp_ptr);
	
	tmp->baseWheight += s.operWheight;
	updateStartPoint(tokens,s,tmp->baseWheight);	
	tokens.push_back(tmp.ptr());
        if(show)
            pushItem3(token_list::type::FUNC,tmp->baseWheight,s,str.size()-1);
        s.operWheight+=5;
        s.fstack.push(s.operWheight);
        tokens.push_back(new token::parenthesesToken(true));
	tmp.release();
}

void parse_var(std::string str,
		tvec& tokens,
		i_state& s, 
		math_func::function_interface* func,
		memory::memory* mem)
{

	token::variableToken * tmp = new token::variableToken(mem,func);
	tmp->variableName = str;
	tokens.push_back(tmp);
        pushItem3(token_list::type::VAR,s.operWheight,s,str.size());
}

}

