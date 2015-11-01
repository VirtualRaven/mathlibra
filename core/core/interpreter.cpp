#include "interpreter.h"
#include "ptr_protect.h"
#include "function_obj.h"
#include "exception_helper.h"

/**
 * Exception thrower for intepreter
 */
template<EXCEPTION T> void interpreterOops()
{
	__mathlibra__raise<T,INTERPRETER>();
}






	bool PNegativeDigit(std::vector<token::baseToken*>& tokens, char ** expression, unsigned short i)
{
	if (!tokens.empty())
	{
		token::baseToken * token = tokens.back();
		if (token->type == tree::PARENTHESES)
		{
			return ((*expression)[i - 1] == '(');

		}
		else if (token->type == tree::OPERATOR)
		{
			return true;
		}
		else return false;

	}
	else return true;
}





	class interpreter;


void debug::check_tree_mem_leak()
{
	util::object_stats<tree::nodeDataInterface> stats;
	std::cout << "COUNTABLE ASSERT RUNNING\n" << "STATS: \n" << "total: " << stats.get_total() << "\ncurrent: " << stats.get_current() << "\ndeleted: " << stats.get_deleted() << std::endl;
	if(stats.get_total() != stats.get_deleted())
	{
		interpreterOops<MEMORY_LEAK_ASSERT>();
	}	
}



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
	void interpreter::allocExpression(short lenght )
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
		this->destroyTokens();
		tokens.reserve(expressionLength); 
		short extraOperatorWheight =0;
		this->startOperatorPos = 0;
		size_t lowestWheight = 9999;
		std::stack<token::parenthesesToken *> parStack;
		for(size_t i = 0; i< expressionLength; i++)
		{
			if(expression[i] == '(')
			{
				short startPos = i;
				if (tokens.size() > 0 && (tokens.back()->type == tree::VALUE || tokens.back()->type == tree::VARIABLE) && this->_operators->inArray('*')) //If priviouse value or variable is an value and an multiplication sign
				{
					ptr_protect<token::operatorToken*, false> tmp(new token::operatorToken(_operators->getCurrent()));
					tmp->startPos = startPos;
					tmp->endPos = startPos;
					startPos += 1;
					tmp->baseWheight += extraOperatorWheight;
					if (tmp->baseWheight <= lowestWheight) // <= for left association && < for right association
					{
						lowestWheight = tmp->baseWheight;
						this->startOperatorPos = tokens.size();
					}
					tokens.push_back(tmp.ptr());
					tmp.release();
				}

				tokens.push_back(new token::parenthesesToken(startPos, startPos));
				parStack.push((token::parenthesesToken*)tokens.back());
				extraOperatorWheight+=5;
				continue;
			}
			else if(expression[i] == ')')
			{
				if(parStack.empty())
				{
					interpreterOops<SYNTAX_UNMATCHED_CLOSING_PARANTHESES>();
					
				}
				else
				{
					token::parenthesesToken* tmp = parStack.top();
					parStack.pop();
					tmp->opposit=i;
					token::parenthesesToken * tmp2 = new token::parenthesesToken(i, i);
					tmp2->opposit = tmp->startPos;
					tokens.push_back(tmp2);
					extraOperatorWheight-=5;
					continue;

				}
			}
			else if(isdigit(expression[i]))
			{
				ptr_protect<token::valueToken*, false> tmp(new token::valueToken(i, 0));
				short valueLength=0;
				for(unsigned int i2 =i+1; i2 < expressionLength; i2++)
				{
					if( !isdigit(expression[i2]) && expression[i2] !='.' && expression[i2] != 'e' && expression[i2] != '-' )
					{
						break;
					}
					else if(expression[i2] == 'e')
					{
						if(i2+1 < expressionLength && ( !isdigit(expression[i2+1]) && expression[i2+1] !='-') )
						{
							interpreterOops<SYNTAX_EXP_AFTER_E>();
						}
						else if(i2+1 >=expressionLength)
						{
							interpreterOops<SYNTAX_EXP_AFTER_E>();
						}

					}
					else if(expression[i2] == '-')
					{
						if(i2-1 < expressionLength  && expression[i2-1] != 'e') //If expression is > instead of >= it will fail for expressions as 0-x^2
						{
							break;
						}

					}
					valueLength++;
				}
				tmp->endPos = i+valueLength;
				short tmp_str_length = tmp->endPos+1 - tmp->startPos;

				//Create a temporary string from which we can convert the string to a double
				char* tmp_str = nullptr;
                		tmp_str = new char[tmp_str_length+1];
				memcpy(tmp_str, &expression[i], (tmp_str_length*sizeof(char)) );
				tmp_str[tmp_str_length] = '\0';
				tmp->value =  atof(tmp_str);
				delete[] tmp_str;
				tokens.push_back(tmp.ptr());
				tmp.release(); //Release ownership of pointer
				i+=valueLength;
				continue;
			}
			else if (expression[i] == '-' && PNegativeDigit(tokens, &expression,i) && (i + 1) < expressionLength && isdigit(expression[i+1]) )
			{
				ptr_protect<token::valueToken*, false> tmp(new token::valueToken(i, 0));
				short valueLength = 0;
				for (unsigned int i2 = i + 1; i2 < expressionLength; i2++)
				{
					if (!isdigit(expression[i2]) && expression[i2] != '.' && expression[i2] != 'e' && expression[i2] != '-')
					{
						break;
					}
					else if (expression[i2] == 'e')
					{
						if (i2 + 1 < expressionLength && !isdigit(expression[i2 + 1]))
						{

							interpreterOops<SYNTAX_EXP_AFTER_E>();
						}

					}
					else if(expression[i2] == '-')
					{
						if(i2-1 > 0 && expression[i2-1] != 'e')
						{
							break;
						}

					}
					valueLength++;
				}
				tmp.ptr()->endPos = i + valueLength;
				short tmp_str_length = (tmp.ptr()->endPos + 1) - tmp.ptr()->startPos;

				//Create a temporary string from which we can convert the string to a double
				char* tmp_str = new char[tmp_str_length + 1];
				memcpy(tmp_str, &expression[i], (tmp_str_length*sizeof(char)));
				tmp_str[tmp_str_length] = '\0';
				tmp.ptr()->value = atof(tmp_str);
				delete[] tmp_str;
				tokens.push_back(tmp.ptr());
				tmp.release();
				i += valueLength;
				continue;
			}

			else if (this->_operators != nullptr && this->_operators->inArray(expression[i]))
			{
				if (expression[i] == '-') //Following test compensates for implicit -1 situations like -x or 2*-(2+x) which will be extended to -1*x and 2*-1*(2+x) where the multiplication sign between -1 and the other terme has an higher than usal weight
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

					if ( (i - 1 > 0 && (test_type == tree::OPERATOR || expression[i - 1] == '(')) || i == 0) 
					{
						if (this->_operators->inArray('*'))
						{

							ptr_protect<token::valueToken*, false> tmp_val(new token::valueToken(i, i));
							tmp_val->value = -1;
							tokens.push_back(tmp_val.ptr());
							tmp_val.release();
							ptr_protect<token::operatorToken *, false> tmp = new  token::operatorToken(_operators->getCurrent());
							tmp->startPos = i;
							tmp->endPos = i;
							tmp->baseWheight += extraOperatorWheight + 5;
							if (tmp->baseWheight <= lowestWheight) // <= for left association && < for right association
							{
								lowestWheight = tmp->baseWheight;
								this->startOperatorPos = tokens.size();
							}
						
							tokens.push_back(tmp.ptr());
							tmp.release();
							continue;
						}
					}
				}

				ptr_protect<token::operatorToken*, false> tmp(new token::operatorToken(_operators->getCurrent()));
				tmp->startPos=i;
				tmp->endPos=i;
				tmp->baseWheight +=extraOperatorWheight;
				if(tmp->baseWheight <= lowestWheight) // <= for left association && < for right association
				{
					lowestWheight = tmp->baseWheight;
					this->startOperatorPos =tokens.size();
				}
				if (tmp->operChar == '=')
				{

					if (tokens.back()->type != tokenType::VARIABLE)
					{
						interpreterOops<SYNTAX_ASSIGMENT_NEEDS_VAR>();
					}
					else
					{
						//token is going to be used in an asigment and should thus not be bushed to the stack
						static_cast<token::variableToken*>(tokens.back())->_stack = false;
					}
				}
				tokens.push_back(tmp.ptr());
				tmp.release();
				continue;

			}
			else if (isalpha(expression[i]) )
			{
					
					unsigned short valueLength = 0;
					unsigned short endPos = 0;
					unsigned short startPos = i;
					std::string name;
					for (unsigned short i2 = i + 1; i2 < expressionLength; i2++)
					{

						if (!isalpha(expression[i2]))
						{
							break;
						}
						valueLength++;

					}
					endPos = i + valueLength;
					name = std::string(expression, startPos, valueLength +1 );
					i += valueLength;
					
					if (tokens.size() > 0 && tokens.back()->type == tree::VALUE &&  this->_operators->inArray('*')) //If priviouse value is an value and an multiplication sign
					{
						ptr_protect<token::operatorToken*, false> tmp(new token::operatorToken(_operators->getCurrent()));
						tmp->startPos = startPos;
						tmp->endPos = startPos;
						startPos += 1;
						endPos += 1;
						tmp->baseWheight += extraOperatorWheight;
						if (tmp->baseWheight <= lowestWheight) // <= for left association && < for right association
						{
							lowestWheight = tmp->baseWheight;
							this->startOperatorPos = tokens.size();
						}
						tokens.push_back(tmp.ptr());
						tmp.release();
					}
					if (current_functions != nullptr && current_functions->isloaded(name).loaded)
					{
						token::funcToken * tmp_ptr;
                                                func_type type = current_functions->type();
						if (type == math_func::func_type::GENERAL )
						{
							tmp_ptr = new token::funcToken(startPos, endPos, (token::funcToken::generalFuncPtr)current_functions->get(name));
						}
						else if (type == math_func::func_type::FAST)
						{
							tmp_ptr = new token::funcToken(startPos, endPos, (token::funcToken::funcPtr)current_functions->get(name));
						}
                                                else
                                                {
                                                    tmp_ptr = new token::funcToken(startPos,endPos,current_functions->getFunctor(name));

                                                }

						ptr_protect<token::funcToken *, false> tmp(tmp_ptr);
						
						tmp->baseWheight += extraOperatorWheight;
						if (tmp->baseWheight <= lowestWheight) // <= for left association && < for right association
						{
							lowestWheight = tmp->baseWheight;
							this->startOperatorPos = tokens.size();
						}
						tokens.push_back(tmp.ptr());
						tmp.release();
					}
					else if (mem != nullptr)
					{
						token::variableToken * tmp = new token::variableToken(startPos, endPos, mem,current_functions);
						tmp->variableName = name;
						tokens.push_back(tmp);
					}
					else
					{
						interpreterOops<VARIABLES_DISABLED_BUT_USED>();
					}

					continue;
			}
			else
			{
				interpreterOops<UNKNOWN_CHAR_STR>();
			}



		}
		if(!parStack.empty())
		{
			interpreterOops<SYNTAX_UNMATCHED_OPENING_PARANTHESES>();
		}
		else
		{
			tokens.shrink_to_fit();
		}


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
			else if(this->tokens.size() == 1 && (this->tokens[0]->type == tokenType::VALUE || this->tokens[0]->type == tokenType::VARIABLE ))
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
	number_type interpreter::exec()
	{
        if(!rootEmpty)
        {
#ifdef STRUCTUAL_INTEGRITY_TEST
            this->root.integrityTest();
#endif
			this->root.TakeContext();
			return this->root.data->eval();

        }
		interpreterOops<EXEC_UNFINISHED_EXPR>();
		return 0;
	}
	void interpreter::set(const char * expression_, unsigned short lenght)
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
	interpreter::interpreter(const interpreter& other)
	{
		interpreterOops<CANT_CPY_OBJ>();
	}
	interpreter& interpreter::operator=(const interpreter& other)
	{
		return *this;
	}
	interpreter& interpreter::operator=(interpreter&& other)
	{
		return *this;
	}
