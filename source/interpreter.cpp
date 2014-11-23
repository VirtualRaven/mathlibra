#include "interpreter.h"

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





	buildTree::buildTree(interpreter * tgt)
	{
		parentInterpreter= tgt;
		 built=false;
	}

	buildTree::~buildTree()
	{

	}
	rootNode buildTree::getTree()
	{
	    if(built)
        {
          return this->root;
        }
		else throw interpreterOops("-[ Tried to retreiv an incomplete tree ]");

	}
	bool buildTree::buildEntry1()
	{
            built=false;
			if (parentInterpreter->tokens.size() <= parentInterpreter->startOperatorPos)
			{
				throw interpreterOops("Panic: Tokens subscript operator out of bounds");
			}
			mathNode::mathExpressionNode* topNode = parentInterpreter->tokens[parentInterpreter->startOperatorPos]->node(); //Create the top node out of the starting token

			this->root.set(topNode); // Put the node into the root of the tree
			buildVector build(0, parentInterpreter->tokens.size(), parentInterpreter->startOperatorPos,&parentInterpreter->tokens); //Fill the build vector
			if (!buildSubNodes(static_cast<mathNode::mathExpressionNode*>(root.data),build)) //Build the sub node acording to the build vector
			{
				std::cerr << "-[ Failed to build syntax tree ]\n";
				root.deleteSubNodes();
                root.data->destroy();
                root.data=0;
                built=false;
                return built;
			}
			else
			{

                built=true;

                #ifdef DEBUG
                    this->root.integrityTest();
                #endif
                return built;
			}


	}





	void interpreter::emptyRoot()
	{
	    if(!rootEmpty)
        {
            root.deleteSubNodes();
            root.data->destroy();
            root.data=0;
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
		char * tmp_buffer = new char[ expressionLength];
		short tmp_buffer_index =0;
		for(int i = 0; i < expressionLength; i++)
		{
			if(expression[i] != ' ' )
			{
				tmp_buffer[tmp_buffer_index] = expression[i];
				tmp_buffer_index++;
			}

		}

		allocExpression(tmp_buffer_index);
		memcpy(expression,tmp_buffer,tmp_buffer_index*sizeof(char));
		delete[] tmp_buffer;
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
		expression = new char[lenght];
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

				}
			}
			else if(isdigit(expression[i]))
			{
				valueToken * tmp = new valueToken(i,0);
				short valueLength=0;
				for(int i2 =i+1; i2 < expressionLength; i2++)
				{
					if( !isdigit(expression[i2]) && expression[i2] !='.' && expression[i2] != 'e')
					{
						break;
					}
					else if(expression[i2] == 'e')
					{
						if(i2+1 < expressionLength && !isdigit(expression[i2+1]) )
						{
							std::cerr << "Syntax error: expected exponent after e \n";
							return false;
						}

					}
					valueLength++;
				}
				tmp->endPos = i+valueLength;
				short tmp_str_length = (tmp->endPos+1)-tmp->startPos;

				//Create a temporary string from which we can convert the string to a double
				char* tmp_str = new char[tmp_str_length+1];
				memcpy(tmp_str, &expression[i], (tmp_str_length*sizeof(char)) );
				tmp_str[tmp_str_length] = '\0';
				tmp->value =  atof(tmp_str);
				delete[] tmp_str;
				tokens.push_back(tmp);
				i+=valueLength;
			}
			else if (expression[i] == '-' && PNegativeDigit(tokens, &expression,i) &&i + 1 < expressionLength && isdigit(expression[i+1]) )
			{
				valueToken * tmp = new valueToken(i, 0);
				short valueLength = 0;
				for (int i2 = i + 1; i2 < expressionLength; i2++)
				{
					if (!isdigit(expression[i2]) && expression[i2] != '.' && expression[i2] != 'e')
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
					valueLength++;
				}
				tmp->endPos = i + valueLength;
				short tmp_str_length = (tmp->endPos + 1) - tmp->startPos;

				//Create a temporary string from which we can convert the string to a double
				char* tmp_str = new char[tmp_str_length + 1];
				memcpy(tmp_str, &expression[i], (tmp_str_length*sizeof(char)));
				tmp_str[tmp_str_length] = '\0';
				tmp->value = atof(tmp_str);
				delete[] tmp_str;
				tokens.push_back(tmp);
				i += valueLength;
			}

			else if(operators.inArray(expression[i]))
			{
				operatorToken* tmp = new operatorToken(operators.getCurrent());
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
				}
				tokens.push_back(tmp);

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
					name = std::string(expression, startPos, valueLength + 1);

					i += valueLength;
					if (current_functions != nullptr && current_functions->isloaded(name) == true)
					{
						funcToken *tmp = new funcToken(startPos, endPos, current_functions->get(name));
						tmp->baseWheight += extraOperatorWheight;
						if (tmp->baseWheight <= lowestWheight) // <= for left association && < for right association
						{
							lowestWheight = tmp->baseWheight;
							this->startOperatorPos = tokens.size();
						}
						tokens.push_back(tmp);
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
		buildTree buildObject(this);


		if(this->startOperatorPos)
		{
			if(buildObject.buildEntry1())
            {
               emptyRoot();
               this->root =buildObject.getTree();
               rootEmpty = false;
               return true;

            }
            else
            {
				std::cerr << "Failed to build tree\n";
				return false;
            }
		}
		else
		{
			if (this->tokens[this->startOperatorPos]->type == FUNCTION)
			{
				if (buildObject.buildEntry1())
				{
					emptyRoot();
					this->root = buildObject.getTree();
					rootEmpty = false;
					return true;

				}
				else
				{
					std::cerr << "Failed to build tree\n";
					return false;
				}
			}
			std::cerr << "Can't find start point\n";
				return false;
		}

	}




	void interpreter::setMemory(memory* mem)
	{
		this->mem = mem;
	}
	bool interpreter::interpret()
	{
		//Add syntax checking
		//std::cerr << "-[Interptating: " << this->expression << "\n";


		if (!lexicalAnalys())
		{
			return false;
		}
#ifdef DEBUG
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
#endif //DEBUG
		// if 0 == this->startOperatorPos we need a way to figure out where to begin if no operators exists, e.g expression sqrt(3)



		if (!this->buildSyntaxTree())
		{
			this->destroyTokens(); //Remove all tokens
			return false;
		}
		this->destroyTokens(); //Remove all tokens
        #ifdef DEBUG
            this->root.integrityTest();
        #endif

		return true;

	}
	number_type interpreter::exec()
	{
        if(!rootEmpty)
        {
            #ifdef DEBUG
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
	{
		mem = nullptr;
		expression=nullptr;
		rootEmpty=true;
		expressionLength=0;
		startOperatorPos=0;

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
