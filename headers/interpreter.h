#ifndef interpreter_H_INCLUDED
#define interpreter_H_INCLUDED
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <vector>
#include "build.h"
#include "functions.h"

namespace enums
{
	enum fixed_chars{ NEGATIVE_SIGN = 1, UNKNOWN = 0 };
};
struct interpreterOops : public exception
{
	interpreterOops(std::string inf)
	{
		info = inf;
	}

	const char* what()
	{
		return "interpreter Exception";
	}
	
};


template<typename mathType>
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





template<typename mathType>	class interpreter;



template<typename mathType>
class buildTree
{
	interpreter<mathType>* parentInterpreter;
	rootNode root;
	bool built;

public:
	buildTree(interpreter<mathType> * tgt)
	{
		parentInterpreter= tgt;
		 built=false;
	}

	~buildTree()
	{

	}
	rootNode getTree()
	{
	    if(built)
        {
          return this->root;
        }
		else throw interpreterOops("-[ Tried to retreiv an incomplete tree ]");

	}
	bool buildEntry1()
	{
            built=false;
			if (parentInterpreter->tokens.size() <= parentInterpreter->startOperatorPos)
			{
				throw interpreterOops("Panic: Tokens subscript operator out of bounds");
			}
			mathNode::mathExpressionNode* topNode = parentInterpreter->tokens[parentInterpreter->startOperatorPos]->node();

			this->root.set(topNode);
			buildVector<mathType> build(0, parentInterpreter->tokens.size(), parentInterpreter->startOperatorPos,&parentInterpreter->tokens);
			if (!buildSubNodes<mathType>(static_cast<mathNode::mathExpressionNode*>(root.data),build))
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


};


template<typename mathType>
class interpreter
{

	char * expression;
	short expressionLength;
	short startOperatorPos; //contains the index in the tokens array where the starting operator is located
	operators::allOperators<mathType> operators;

	friend buildTree<mathType>;
	std::vector<baseToken*> tokens;

	rootNode root;
	math_func::function_interface<double> current_functions;
	memory<mathType> *mem;
	bool rootEmpty;
	void emptyRoot()
	{
	    if(!rootEmpty)
        {
            root.deleteSubNodes();
            root.data->destroy();
            root.data=0;
        }
	}
	void destroyTokens()
	{
		for(unsigned int i = 0; i < this->tokens.size(); i++)
		{
			delete	(baseToken*)this->tokens[i];
			this->tokens[i] = nullptr;
		}
		tokens.clear();

	}

	void stripSlashes()
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
	void freeExpression()
	{
		if(expression)
		{
			delete[] expression;
			expression =nullptr;
			expressionLength =0;
		}
	}
	void allocExpression(short lenght )
	{
		if(expression)
		{
			freeExpression();
		}
		expressionLength= lenght;
		expression = new char[lenght];
	}

	bool lexicalAnalys() 
	{
		this->destroyTokens();
		short extraOperatorWheight =0;
		this->startOperatorPos = 0;
		short lowestWheight = 9999;
		std::stack<parenthesesToken<mathType> *> parStack;
		for(int i = 0; i< expressionLength; i++)
		{
			if(expression[i] == '(')
			{

				tokens.push_back(new parenthesesToken<mathType>(i,i));
				parStack.push( (parenthesesToken<mathType>*)tokens.back() );
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
					parenthesesToken<mathType>* tmp = parStack.top();
					parStack.pop();
					tmp->opposit=i;
					parenthesesToken<mathType> * tmp2 = new parenthesesToken<mathType>(i,i);
					tmp2->opposit = tmp->startPos;
					tokens.push_back(tmp2);
					extraOperatorWheight-=4;

				}
			}
			else if(isdigit(expression[i]))
			{
				valueToken<mathType> * tmp = new valueToken<mathType>(i,0);
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
			else if (expression[i] == '-' && PNegativeDigit<mathType>(tokens, &expression,i) &&i + 1 < expressionLength && isdigit(expression[i+1]) )
			{
				valueToken<mathType> * tmp = new valueToken<mathType>(i, 0);
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
				operatorToken<mathType>* tmp = new operatorToken<mathType>(operators.getCurrent());
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
					if (current_functions.isloaded(name) == true)
					{
						tokens.push_back(new funcToken<mathType>(startPos, endPos, current_functions.get(name)));
					}	
					else if (mem != nullptr)
					{
						variableToken<mathType> * tmp = new variableToken<mathType>(startPos, endPos, mem);
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

	bool buildSyntaxTree()
	{
		buildTree<mathType> buildObject(this);


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
			std::cerr << "Can't find start point\n";
				return false;
		}

	}




public:
	/*Warning the interpeter does not copy the memory object. 
	Therefore must the pointer remain valid throug the lifetime of the interpreter object*/
	void setMemory(memory<mathType>* mem)
	{
		this->mem = mem;
	}
	bool interpret()
	{
		//Add syntax checking
		//std::cerr << "-[Interptating: " << this->expression << "\n";
	
		
		if (!lexicalAnalys())
		{
			return false;
		}
		/*
		for(unsigned int i = 0; i < this->tokens.size(); i++)
		{
			std::cerr <<"-[Token> start: "<< this->tokens[i]->startPos<<" end: " << this->tokens[i]->endPos << " ]\n";
			std::cerr << "-[Type: ";
			switch (this->tokens[i]->type)
			{
				case PARENTHESES:
				{
					std::cerr << "parantheses token]\n";
					parenthesesToken<mathType>* tmp = static_cast<parenthesesToken<mathType>*>( this->tokens[i]);
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
					valueToken<mathType>* tmp = static_cast<valueToken<mathType>*>(this->tokens[i]);
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
					operatorToken<mathType> * tmp = static_cast<operatorToken<mathType>*>(this->tokens[i]);
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
			std::cerr << "\n-[Start pos: " << this->startOperatorPos << " ]\n";
		}
		*/
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
	mathType exec()
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
	void set(const char * expression, short lenght)
	{
		if (lenght > 0)
		{

			allocExpression(lenght);
			memcpy(this->expression, expression, lenght*sizeof(char));
			this->stripSlashes();
		}
		else throw interpreterOops("Passed empty expression to interpreter!\n");
	}

	interpreter()
	{
		mem = nullptr;
		expression=nullptr;
		rootEmpty=true;
		expressionLength=0;
		startOperatorPos=0;

	}

	~interpreter()
	{
		freeExpression();
		destroyTokens();
	}
};

#endif
