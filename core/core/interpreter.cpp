#include "interpreter.h"
#include "ptr_protect.h"

interpreterOops::interpreterOops(std::string inf, bool isCritical) : exception(inf,isCritical){}

	const char* interpreterOops::what()
	{
		return "interpreter Exception";
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
		throw interpreterOops("ASSERT FAILIURE, possible memory leak in program",true);
	}	
}



 rootNode buildEntry1(interpreter * parentInterpreter)
{
           rootNode root;
			if (parentInterpreter->tokens.size() <= parentInterpreter->startOperatorPos)
			{
				throw interpreterOops("Panic: Tokens subscript operator out of bounds", true);
			}
			tree::nodeDataInterface* topNode = parentInterpreter->tokens[parentInterpreter->startOperatorPos]->node(); //Create the top node out of the starting token

			root.set(topNode); // Put the node into the root of the tree
			buildVector build(0, parentInterpreter->tokens.size(), parentInterpreter->startOperatorPos,&parentInterpreter->tokens); //Fill the build vector
			if (!buildSubNodes(static_cast<tree::nodeDataInterface*>(root.data), build)) //Build the sub node acording to the build vector
			{

				root.deleteSubNodes();
                root.data->destroy();
                root.data=0;
                throw interpreterOops("Failed to build syntax tree",false);
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
            root.data->destroy();
            root.data=nullptr;
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

	bool interpreter::lexicalAnalys()
	{
		this->destroyTokens();
		tokens.reserve(expressionLength); 
		short extraOperatorWheight =0;
		this->startOperatorPos = 0;
		size_t lowestWheight = 9999;
		std::stack<token::parenthesesToken *> parStack;
		for(unsigned short i = 0; i< expressionLength; i++)
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
					std::cerr << "Syntax error : found unmatched ) \n";
					return false;
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
							std::cerr << "Syntax error: expected exponent after e \n";

							return false;
						}

					}
					else if(expression[i2] == '-')
					{
						if(i2-1 >= 0 && expression[i2-1] != 'e') //If expression is > instead of >= it will fail for expressions as 0-x^2
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
			else if (expression[i] == '-' && PNegativeDigit(tokens, &expression,i) &&i + 1 < expressionLength && isdigit(expression[i+1]) )
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
							std::cerr << "Syntax error: expected exponent after e \n";

							return false;
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
						std::cerr << "Syntax error: Assigment operator requirers an variable on left hand side\n";
						return false;
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
					
					short valueLength = 0;
					short endPos = 0;
					short startPos = i;
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


					if (current_functions != nullptr && current_functions->isloaded(name) == true)
					{
						token::funcToken * tmp_ptr;
						if (current_functions->isGeneral())
						{
							tmp_ptr = new token::funcToken(startPos, endPos, (token::funcToken::generalFuncPtr)current_functions->get(name));
						}
						else
						{
							tmp_ptr = new token::funcToken(startPos, endPos, (token::funcToken::funcPtr)current_functions->get(name));
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
						token::variableToken * tmp = new token::variableToken(startPos, endPos, mem);
						tmp->variableName = name;
						tokens.push_back(tmp);
					}
					else
					{
						std::cerr << "Found variable in string but no memory unit is assigned to the interpeter\n";
						return false;
					}
					continue;
			}
			else
			{
				std::cerr << "Syntax error: found unknown token in string \n";
				return false ;
			}



		}
		if(!parStack.empty())
		{
			std::cerr << "Syntax error: found unmatched ( \n";
			return false ;
		}
		else
		{
			tokens.shrink_to_fit();
			return true;
		}


	}

	bool interpreter::buildSyntaxTree()
	{
        if(this->startOperatorPos)
        {
            emptyRoot();
            this->root =buildEntry1(this);
            rootEmpty = false;
            return true;

        }
		else
		{
			if (this->tokens[this->startOperatorPos]->type == tree::FUNCTION)
			{
					emptyRoot();
					this->root = buildEntry1(this);
					rootEmpty = false;
					return true;
			}
			std::cerr << "Can't find start point\n";
            return false;
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
	bool interpreter::interpret()
	{
		//Add syntax checking
		//std::cerr << "-[Interptating: " << this->expression << "\n";


		if (!lexicalAnalys())
		{
			return false;
		}
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



		if (!this->buildSyntaxTree())
		{
			this->destroyTokens(); //Remove all tokens
			return false;
		}
		this->destroyTokens(); //Remove all tokens
        #ifdef STRUCTUAL_INTEGRITY_TEST
            this->root.integrityTest();
        #endif

		return true;

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
		else throw interpreterOops("Tried to execute unfinished expresion",false);
	}
	void interpreter::set(const char * expression_, unsigned short lenght)
	{
		if (lenght > 0)
		{

			allocExpression(lenght);
			memcpy(this->expression, expression_, lenght*sizeof(char));
			this->stripSlashes();
		}
		else throw interpreterOops("Passed empty expression to interpreter!\n",false);
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
#ifdef ENABLE_CORAX
	void CoraxVM::Corax_program_builder_module::_rpn(node * nodePtr, CoraxVM::corax_program * prgm )
	{
	    if(nodePtr == nullptr)
        {
            return;
        }
	    else if(nodePtr->data->type == tokenType::VALUE || nodePtr->data->type == tokenType::VARIABLE  )
        {
			/* 
				LDI R1 CONST
				PUSH R1
			*/
            if(nodePtr->data->type == tokenType::VALUE)
            {
               prgm->instructions.push_back(CoraxVM::corax_program_instruction(CoraxVM::instruction_set::LDI | CoraxVM::instruction_flags::R1,
                                                                               static_cast<mathNode::mathExpressionNode_val*>(nodePtr->data)->value ));
               prgm->instructions.push_back(CoraxVM::corax_program_instruction(CoraxVM::instruction_set::PUSH | CoraxVM::instruction_flags::R1, nullptr));
              // std::cout << static_cast<mathNode::mathExpressionNode_val*>(nodePtr->data)->value;
#ifdef DEBUG_CORAX_INS
			   std::cout << "LDI R1 " <<  static_cast<mathNode::mathExpressionNode_val*>(nodePtr->data)->value <<"\nPUSH R1\n";
#endif
            }
			/*
				LD R1 CONST_PTR
				PUSH R1
			*/
            else
            {
				mathNode::mathExpressionNode_variable* tmp_math_var = static_cast<mathNode::mathExpressionNode_variable*>(nodePtr->data);
				
				if (tmp_math_var->is_pushable())
				{
					void* tmp_ptr = (void*)this->_ptr->mem->raw_ptr(tmp_math_var->name);
					prgm->instructions.push_back(CoraxVM::corax_program_instruction(CoraxVM::instruction_set::LD | CoraxVM::instruction_flags::R1, tmp_ptr));
					prgm->instructions.push_back(CoraxVM::corax_program_instruction(CoraxVM::instruction_set::PUSH | CoraxVM::instruction_flags::R1, nullptr));
					#ifdef DEBUG_CORAX_INS
					std::cout << "LD R1 " << tmp_ptr << "\nPUSH R1\n";
					#endif
				}
				else
				{
					if (!this->_ptr->mem->set(tmp_math_var->name, 0))
					{
						throw CoraxVM::coraxOops("Failed to allocate memory for variable for assigment operator");
					}
					void* tmp_ptr = (void*)this->_ptr->mem->raw_ptr(tmp_math_var->name);
					prgm->instructions.push_back(CoraxVM::corax_program_instruction(CoraxVM::instruction_set::LDI | CoraxVM::instruction_flags::PR2, tmp_ptr));
#ifdef DEBUG_CORAX_INS
					std::cout << "LDI PR2 " << tmp_ptr << "\n";
#endif
				}
				//std::cout << tmp_math_var->name;
			
            }
            return;
        }
        else if(nodePtr->data->type == tokenType::UNKNOWN)
        {
            throw interpreterOops("COMPILER FAILURE: found unknown node type",false);
        }
        else
        {
            _rpn(nodePtr->sub1(),prgm);
            _rpn(nodePtr->sub2(),prgm);
			/*
				CALL ARG1 CONST_PTR
			*/
            if(nodePtr->data->type == tokenType::FUNCTION)
            {
                prgm->instructions.push_back(CoraxVM::corax_program_instruction(CoraxVM::instruction_set::CALL | CoraxVM::instruction_flags::ARG1, 
                                                                            (void*)static_cast<mathNode::mathExpressionNode_func*>(nodePtr->data)->func));
#ifdef DEBUG_CORAX_INS
				std::cout << "CALL ARG1 " <<  (void*)static_cast<mathNode::mathExpressionNode_func*>(nodePtr->data)->func << "\n";
#endif

               // std::cout << "FUNCTION(" << std::hex << (void *)static_cast<mathNode::mathExpressionNode_func*>(nodePtr->data)->func<< std::dec << ")";
            }
			/*
				CALL ARG2 CONST_PTR
			*/
            else //Is operator
            {
                mathNode::mathExpressionNode_opr * tmp = static_cast<mathNode::mathExpressionNode_opr*>(nodePtr->data);
                if(tmp->assignB)
                {
					prgm->instructions.push_back(CoraxVM::corax_program_instruction(CoraxVM::instruction_set::POP | CoraxVM::instruction_flags::R2,nullptr));
					prgm->instructions.push_back(CoraxVM::corax_program_instruction(CoraxVM::instruction_set::ST | CoraxVM::instruction_flags::R2 | CoraxVM::instruction_flags::PR2, nullptr));
					prgm->instructions.push_back(CoraxVM::corax_program_instruction(CoraxVM::instruction_set::PUSH  |CoraxVM::instruction_flags::R2, nullptr));
#ifdef DEBUG_CORAX_INS
					std::cout << "POP R2\nST R2 PR2\nPUSH R2\n";
#endif
                //    std::cout << "ASSIGN(" << std::hex<< (void *)tmp->assign << std::dec<< ")";
                }
                else
                {
                    prgm->instructions.push_back(CoraxVM::corax_program_instruction(CoraxVM::instruction_set::CALL | CoraxVM::instruction_flags::ARG2, 
                                                                            (void*)tmp->operation));
#ifdef DEBUG_CORAX_INS
					std::cout << "CALL ARG2 " <<  (void*)tmp->operation << "\n";
#endif
               //   std::cout << "OPER("<< std::hex << (void *)tmp->operation<< std::dec << ")";
                }
            }

        }
	}

	bool CoraxVM::Corax_program_builder_module::create_program(interface::corax_program * prgm)
	{
	    CoraxVM::corax_program * cast_prgm = dynamic_cast<CoraxVM::corax_program*>(prgm);
	    if(this->_ptr->rootEmpty)
        {
            return false;
        }
	     //Create rpn
		cast_prgm->clear();
		_rpn(&this->_ptr->root, cast_prgm);
        return true;

	}
	CoraxVM::Corax_program_builder_module::Corax_program_builder_module(interpreter* ptr) : _ptr(ptr){}
#endif //ENABLE_CORAX
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
		throw interpreterOops("Can't copy interpreter object",true);
	}
	interpreter& interpreter::operator=(const interpreter& other)
	{
		return *this;
	}
	interpreter& interpreter::operator=(interpreter&& other)
	{
		return *this;
	}