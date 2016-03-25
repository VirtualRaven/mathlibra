/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "interpreter.h"
#include "ptr_protect.h"
#include "function_obj.h"
#include "exception_helper.h"
#include "lexical.h"
#include <algorithm>
/**
 * Exception thrower for intepreter
 */
template<EXCEPTION T> void interpreterOops()
{
	__mathlibra__raise<T,INTERPRETER>();
}

	class interpreter;



 rootNode buildEntry1(interpreter * parentInterpreter)
{
           rootNode root;
			if (parentInterpreter->tokens.size() <= parentInterpreter->startOperatorPos)
			{
				interpreterOops<SUBSCRIPT_OPERATOR_OUT_OF_BOUNDS>();
			}
			tree::nodeDataInterface* topNode = parentInterpreter->tokens[parentInterpreter->startOperatorPos]->node(); //Create the top node out of the starting token

			root.set(topNode); // Put the node into the root of the tree
			buildVector build(0, parentInterpreter->tokens.size(), parentInterpreter->startOperatorPos,&parentInterpreter->tokens); //Fill the build vector
			if (!buildSubNodes(static_cast<tree::nodeDataInterface*>(root.data), build)) //Build the sub node acording to the build vector
			{

			    root.deleteSubNodes();
                            root.data->destroy();
                            root.data=0;
                            interpreterOops<TREE_BUILD_FAILED>();
			    return root;
			}
			else
			{
#ifdef STRUCTUAL_INTEGRITY_TEST
                            root.integrityTest();
#endif
                            return root;
                        }


	}





	void interpreter::emptyRoot()
	{
	    if(!rootEmpty)
        {
            root.deleteSubNodes();
			if (root.data != nullptr)
			{
				root.data->destroy();
				root.data = nullptr;
			}
            rootEmpty=true;
        }
	}

	void interpreter::stripParanthese()
	{	
		std::vector<token::baseToken*> tmp;
		std::copy_if(	this->tokens.begin(),
				this->tokens.end(),
				std::back_inserter(tmp),
				[](token::baseToken* t){return t->type!=tree::PARENTHESES;}
			    );
		this->tokens = tmp;
				
	}
	void interpreter::destroyTokens()
	{
		unsigned int size=this->tokens.size();
		for (unsigned int i = 0; i < size; i++)
		{
			delete	(token::baseToken*)this->tokens[i];
			this->tokens[i] = nullptr;
		}
		tokens.clear();

	}

	void interpreter::stripSlashes()
	{
		ptr_protect<char*,true> tmp_buffer = safe_alloc<char>(expressionLength);
		short tmp_buffer_index =0;
		for(unsigned int i = 0; i < expressionLength; i++)
		{
			if(expression[i] != ' ' )
			{
				tmp_buffer.ptr()[tmp_buffer_index] = expression[i];
				tmp_buffer_index++;
			}

		}


        allocExpression(tmp_buffer_index);
		memcpy(expression,tmp_buffer.ptr(),tmp_buffer_index*sizeof(char));

	}
	void interpreter::freeExpression()
	{
		if(expression)
		{
			delete[] expression;
			expression =nullptr;
			expressionLength =0;
		}
	}
	void interpreter::allocExpression(size_t lenght )
	{
		if(expression)
		{
			freeExpression();
		}
		expressionLength= lenght;
		expression = new char[lenght+1];
		expression[expressionLength] = '\0';
	}

	void  interpreter::lexicalAnalys()
	{
		auto res = lexical(this->expression, this->expressionLength,this->mem,this->current_functions,this->_operators);
		this->startOperatorPos = res.startPos;
		this->tokens=res.tokens;
	}

	void interpreter::buildSyntaxTree()
	{
       		 if(this->startOperatorPos)
       		 {
       		     emptyRoot();
       		     this->root =buildEntry1(this);
       		     rootEmpty = false;

       		 }
		else
		{
			if (this->tokens[this->startOperatorPos]->type == tree::FUNCTION)
			{
				emptyRoot();
				this->root = buildEntry1(this);
				rootEmpty = false;
			}
			else
			{
				this->stripParanthese();
			 	if( this->tokens.size() == 1 && (this->tokens[0]->type == tokenType::VALUE || this->tokens[0]->type == tokenType::VARIABLE ))
				{
					emptyRoot();
					this->root = buildEntry1(this);
					rootEmpty=false;
				}
				else
				{
					interpreterOops<CANT_FIND_STARTING_POINT>();
				}
			}
		}

	}




	void interpreter::setMemory(memory::memory* mem_)
	{
		this->mem = mem_;
	}
	void interpreter::setOperator(operators::operators_interface* operators)
	{
	    this->_operators = operators;
	}
	void  interpreter::interpret()
	{
		//Add syntax checking
		//std::cerr << "-[Interptating: " << this->expression << "\n";


		lexicalAnalys();
#ifdef  LEXICAL_ANANALYSIS_DEBUG
		for(unsigned int i = 0; i < this->tokens.size(); i++)
		{
			std::cerr <<"-[Token> start: "<< this->tokens[i]->startPos<<" end: " << this->tokens[i]->endPos << " ]\n";
			std::cerr << "-[Type: ";
			switch (this->tokens[i]->type)
			{
			case tree::PARENTHESES:
				{
					std::cerr << "parantheses token]\n";
					parenthesesToken* tmp = static_cast<parenthesesToken*>( this->tokens[i]);
					std::cerr <<"-[Opposit token found at "  << tmp->opposit << "]\n\n";
					break;
				}
			case tree::FUNCTION:
				{
					std::cerr << "function token]\n\n";
					break;
				}
			case tree::VALUE:
				{
					std::cerr << "value token]\n";
					valueToken* tmp = static_cast<valueToken*>(this->tokens[i]);
					std::cerr << "-[Value:" << tmp->value << "]\n\n";
					break;
				}
				case tree::VARIABLE:
				{
					std::cerr << "variable token]\n\n";
					break;
				}
				case tree::OPERATOR:
				{
					std::cerr << "operator token]\n";
					operatorToken * tmp = static_cast<operatorToken*>(this->tokens[i]);
					std::cerr << "-[operator sign: "<< tmp->operChar << " wheight: " << tmp->baseWheight<< "]\n\n";
					break;
				}
				case tree::UNKNOWN:
				{
					std::cerr << "unknown]\n\n";
					break;
				}
			default:
				break;
			}

		}
#endif
			try
			{
				this->buildSyntaxTree();
			}
			catch(...)
			{
				this->destroyTokens(); //Remove all tokens
				throw;
			}
		
		this->destroyTokens(); //Remove all tokens
        #ifdef STRUCTUAL_INTEGRITY_TEST
            this->root.integrityTest();
        #endif


	}
	void interpreter::optimize()
	{	
		
        	if(!rootEmpty)
        	{
			this->root.TakeContext();
			this->root.data->optimize();
			return;
        	}
		interpreterOops<EXEC_UNFINISHED_EXPR>();
	}
        
	interface::type_ptr interpreter::exec()
	{
        if(!rootEmpty)
        {
#ifdef STRUCTUAL_INTEGRITY_TEST
            this->root.integrityTest();
#endif
			this->root.TakeContext();
			return interface::type_ptr(this->root.data->eval());

        }
		interpreterOops<EXEC_UNFINISHED_EXPR>();
		return interface::type_ptr(nullptr);
	}
	void interpreter::set(const char * expression_, size_t lenght)
	{
		if (lenght > 0)
		{

			allocExpression(lenght);
			memcpy(this->expression, expression_, lenght*sizeof(char));
			this->stripSlashes();
		}
		else interpreterOops<EMPTY_EXPR>();
	}

	interpreter::interpreter()
	:expression(nullptr),
	expressionLength(0),
	startOperatorPos(0),
	_operators(nullptr),
    tokens(),
    root(),
    current_functions(nullptr),
    mem(nullptr),
    rootEmpty(true)

	{


	}

	interpreter::~interpreter()
	{
		freeExpression();
		destroyTokens();
	}


	void interpreter::setFunction(math_func::function_interface* functions)
	{
		this->current_functions = functions;
	}
	/*
			char * expression;
	unsigned short expressionLength;
	unsigned short startOperatorPos; //contains the index in the tokens array where the starting operator is located
	operators::operators_interface* _operators;


	std::vector<baseToken*> tokens;

	rootNode root;
	math_func::function_interface* current_functions;
	memory *mem;
	*/
	interpreter::interpreter(interpreter&& other) :
		expression(other.expression),
		expressionLength(other.expressionLength),
		_operators(other._operators),
		tokens(other.tokens),
		root(std::move(other.root)),
		current_functions(other.current_functions),
		mem(other.mem)
	{
		other.expression = nullptr;
		other.expressionLength = 0;
		other.startOperatorPos = 0;
		other.tokens.clear();
	}
	interpreter::interpreter(const interpreter& )
	{
		interpreterOops<CANT_CPY_OBJ>();
	}
	interpreter& interpreter::operator=(const interpreter& )
	{
		return *this;
	}
	interpreter& interpreter::operator=(interpreter&& )
	{
		return *this;
	}
	function_obj::interpreted_func interpreter::createFunction()
	{		
		bool empty = rootEmpty;
		rootNode old;
		if(!empty){
			old= std::move(this->root);
		}
		this->interpret();
		interpreted_func tmp(&this->root,this->mem);
		if(!empty)
		{
			this->root = std::move(old);
			rootEmpty=false;
		}
		else this->emptyRoot();
		return tmp;
	}	
