#include "interpreter.h"
#include "ptr_protect.h"

interpreterOops::interpreterOops(std::string inf)
	{
		info = inf;
	}

	const char* interpreterOops::what()
	{
		return "interpreter Exception";
	}




bool PNegativeDigit(std::vector<baseToken*>& tokens, char ** expression, short i)
{
	if (!tokens.empty())
	{
		baseToken * token = tokens.back();
		if (token->type == PARENTHESES)
		{
			return ((*expression)[i - 1] == '(');

		}
		else if (token->type == OPERATOR)
		{
			return true;
		}
		else return false;

	}
	else return true;
};





	class interpreter;




 rootNode buildEntry1(interpreter * parentInterpreter)
{
           rootNode root;
			if (parentInterpreter->tokens.size() <= parentInterpreter->startOperatorPos)
			{
				throw interpreterOops("Panic: Tokens subscript operator out of bounds");
			}
			mathNode::mathExpressionNode* topNode = parentInterpreter->tokens[parentInterpreter->startOperatorPos]->node(); //Create the top node out of the starting token

			root.set(topNode); // Put the node into the root of the tree
			buildVector build(0, parentInterpreter->tokens.size(), parentInterpreter->startOperatorPos,&parentInterpreter->tokens); //Fill the build vector
			if (!buildSubNodes(static_cast<mathNode::mathExpressionNode*>(root.data),build)) //Build the sub node acording to the build vector
			{

				root.deleteSubNodes();
                root.data->destroy();
                root.data=0;
                throw interpreterOops("Failed to build syntax tree");
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
		for(unsigned int i = 0; i < this->tokens.size(); i++)
		{
			delete	(baseToken*)this->tokens[i];
			this->tokens[i] = nullptr;
		}
		tokens.clear();

	}

	void interpreter::stripSlashes()
	{
		ptr_protect<char*,true> tmp_buffer = safe_alloc<char>(expressionLength);
		short tmp_buffer_index =0;
		for(int i = 0; i < expressionLength; i++)
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
		short extraOperatorWheight =0;
		this->startOperatorPos = 0;
		short lowestWheight = 9999;
		std::stack<parenthesesToken *> parStack;
		for(int i = 0; i< expressionLength; i++)
		{
			if(expression[i] == '(')
			{

				tokens.push_back(new parenthesesToken(i,i));
				parStack.push( (parenthesesToken*)tokens.back() );
				extraOperatorWheight+=4;
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
					parenthesesToken* tmp = parStack.top();
					parStack.pop();
					tmp->opposit=i;
					parenthesesToken * tmp2 = new parenthesesToken(i,i);
					tmp2->opposit = tmp->startPos;
					tokens.push_back(tmp2);
					extraOperatorWheight-=4;
					continue;

				}
			}
			else if(isdigit(expression[i]))
			{
				ptr_protect<valueToken*,false> tmp(new valueToken(i,0));
				short valueLength=0;
				for(int i2 =i+1; i2 < expressionLength; i2++)
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
						if(i2-1 > 0 && expression[i2-1] != 'e')
						{
							break;
						}

					}
					valueLength++;
				}
				tmp.ptr()->endPos = i+valueLength;
				short tmp_str_length = (tmp.ptr()->endPos+1)-tmp.ptr()->startPos;

				//Create a temporary string from which we can convert the string to a double
				char* tmp_str = nullptr;
                tmp_str = new char[tmp_str_length+1];
				memcpy(tmp_str, &expression[i], (tmp_str_length*sizeof(char)) );
				tmp_str[tmp_str_length] = '\0';
				tmp.ptr()->value =  atof(tmp_str);
				delete[] tmp_str;
				tokens.push_back(tmp.ptr());
				tmp.release(); //Release ownership of pointer
				i+=valueLength;
				continue;
			}
			else if (expression[i] == '-' && PNegativeDigit(tokens, &expression,i) &&i + 1 < expressionLength && isdigit(expression[i+1]) )
			{
				ptr_protect<valueToken*,false> tmp(new valueToken(i, 0));
				short valueLength = 0;
				for (int i2 = i + 1; i2 < expressionLength; i2++)
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

			else if(this->_operators!=nullptr && this->_operators->inArray(expression[i]))
			{
				ptr_protect<operatorToken*,false> tmp(new operatorToken(_operators->getCurrent()));
				tmp.ptr()->startPos=i;
				tmp.ptr()->endPos=i;
				tmp.ptr()->baseWheight +=extraOperatorWheight;
				if(tmp.ptr()->baseWheight <= lowestWheight) // <= for left association && < for right association
				{
					lowestWheight = tmp.ptr()->baseWheight;
					this->startOperatorPos =tokens.size();
				}
				if (tmp.ptr()->operChar == '=')
				{

					if (tokens.back()->type != tokenType::VARIABLE)
					{
						std::cerr << "Syntax error: Assigment operator requirers an variable on left hand side\n";
						return false;
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
					for (int i2 = i + 1; i2 < expressionLength; i2++)
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
					if (current_functions != nullptr && current_functions->isloaded(name) == true)
					{
						ptr_protect<funcToken *,false> tmp(new funcToken(startPos, endPos, current_functions->get(name)));
						tmp.ptr()->baseWheight += extraOperatorWheight;
						if (tmp.ptr()->baseWheight <= lowestWheight) // <= for left association && < for right association
						{
							lowestWheight = tmp.ptr()->baseWheight;
							this->startOperatorPos = tokens.size();
						}
						tokens.push_back(tmp.ptr());
						tmp.release();
					}
					else if (mem != nullptr)
					{
						variableToken * tmp = new variableToken(startPos, endPos, mem);
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
		else return true;


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
			if (this->tokens[this->startOperatorPos]->type == FUNCTION)
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




	void interpreter::setMemory(memory* mem)
	{
		this->mem = mem;
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
				case PARENTHESES:
				{
					std::cerr << "parantheses token]\n";
					parenthesesToken* tmp = static_cast<parenthesesToken*>( this->tokens[i]);
					std::cerr <<"-[Opposit token found at "  << tmp->opposit << "]\n\n";
					break;
				}
				case FUNCTION:
				{
					std::cerr << "function token]\n\n";
					break;
				}
				case VALUE:
				{
					std::cerr << "value token]\n";
					valueToken* tmp = static_cast<valueToken*>(this->tokens[i]);
					std::cerr << "-[Value:" << tmp->value << "]\n\n";
					break;
				}
				case VARIABLE:
				{
					std::cerr << "variable token]\n\n";
					break;
				}
				case OPERATOR:
				{
					std::cerr << "operator token]\n";
					operatorToken * tmp = static_cast<operatorToken*>(this->tokens[i]);
					std::cerr << "-[operator sign: "<< tmp->operChar << " wheight: " << tmp->baseWheight<< "]\n\n";
					break;
				}
				case UNKNOWN:
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
		else throw interpreterOops("Tried to execute unfinished expresion");
	}
	void interpreter::set(const char * expression, short lenght)
	{
		if (lenght > 0)
		{

			allocExpression(lenght);
			memcpy(this->expression, expression, lenght*sizeof(char));
			this->stripSlashes();
		}
		else throw interpreterOops("Passed empty expression to interpreter!\n");
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
               //std::cout << static_cast<mathNode::mathExpressionNode_val*>(nodePtr->data)->value;
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
			//	static_cast<mathNode::mathExpressionNode_variable*>(nodePtr->data)->name;
			//	nodePtr->
			//	prgm->instructions.push_back(CoraxVM::corax_program_instruction(CoraxVM::instruction_set::LD |CoraxVM::instruction_flags::R1,  ))
                 //std::cout << static_cast<mathNode::mathExpressionNode_variable*>(nodePtr->data)->name;
            }
            return;
        }
        else if(nodePtr->data->type == tokenType::UNKNOWN)
        {
            throw interpreterOops("COMPILER FAILURE: found unknown node type");
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

                //std::cout << "FUNCTION(" << std::hex << (void *)static_cast<mathNode::mathExpressionNode_func*>(nodePtr->data)->func<< std::dec << ")";
            }
			/*
				CALL ARG2 CONST_PTR
			*/
            else //Is operator
            {
                mathNode::mathExpressionNode_opr * tmp = static_cast<mathNode::mathExpressionNode_opr*>(nodePtr->data);
                if(tmp->assignB)
                {
                    //std::cout << "ASSIGN(" << std::hex<< (void *)tmp->assign << std::dec<< ")";
                }
                else
                {
                    prgm->instructions.push_back(CoraxVM::corax_program_instruction(CoraxVM::instruction_set::CALL | CoraxVM::instruction_flags::ARG2, 
                                                                            (void*)tmp->operation));
#ifdef DEBUG_CORAX_INS
					std::cout << "CALL ARG2 " <<  (void*)tmp->operation << "\n";
#endif
                  //  std::cout << "OPER("<< std::hex << (void *)tmp->operation<< std::dec << ")";
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