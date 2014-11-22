#ifndef MATHNODE_H_INCLUDED
#define MATHNODE_H_INCLUDED
#include "tree.h"
#include "memory.h"
template<typename mathType>	struct buildVector;
template<typename mathType> struct variableToken;
namespace mathNode
{

    struct nodeOops : public exception
    {
        nodeOops(std::string inf)
        {
            info = inf;
        }

        const char* what()
        {
            return "math node Exception";
        }
    };

	struct mathExpressionNode : public nodeDataInterface
	{


		mathExpressionNode()
		{
			type=UNKNOWN;
		}
		void bind(node * context)
		{
			wrapperNode = context;
		}

		void destroy() //Here goes nothing, fingers crossed
		{
			std::cerr << "Delete signal received, self destruct!\n";
			delete this;
		}




	};


	template<typename mathType>
	struct mathExpressionNode_val : public mathExpressionNode
	{
		mathType value;
		mathExpressionNode_val()
		{
			this->type=tokenType::VALUE;
		}
		mathExpressionNode_val(mathType val)
		{
			value = val;
			this->type=VALUE;
		}
		mathType eval()
		{
			return value;
		}


	};

template<typename mathType> struct mathExpressionNode_opr;

template<typename mathType>
	struct mathExpressionNode_variable : public mathExpressionNode
	{

	private:
		memory<mathType>* mem;
		friend mathExpressionNode_opr<mathType>;
	public:
		std::string name;


		mathExpressionNode_variable(std::string var, memory<mathType>* mem)
		{
			name = var;
			this->mem = mem;
			this->type = tokenType::VARIABLE;
		}
		mathType eval()
		{
			return mem->get(name);
		}


	};



	template<typename mathType>
	struct mathExpressionNode_opr : public mathExpressionNode
	{
		mathType (*operation)(mathType,mathType);

		typedef mathType (*operatorPtr)(mathType,mathType);
		typedef mathType (*assigmentPtr)(variableToken<mathType>, mathType);
		assigmentPtr assign;
		bool assignB;
		mathExpressionNode_opr()
		{
			this->type=OPERATOR;
			operation = nullptr;
			assign = nullptr;
			assignB = false;
		}

		mathExpressionNode_opr(operatorPtr operation)
		{
			this->type=OPERATOR;
			this->operation = operation;
			assign = nullptr;
			assignB = false;
		}
		mathExpressionNode_opr(assigmentPtr assign)
		{
			this->type = OPERATOR;
			this->operation = nullptr;
			this->assign = assign;
			assignB = true;
		}


		mathType eval()
		{
			if(this->wrapperNode->sub1() == nullptr || this->wrapperNode->sub2() == nullptr)
			{
				throw nodeOops("Panic: syntax tree has unexpected null pointer");
			}
			if (assignB)
			{
				if (this->wrapperNode->sub1()->data->type == VARIABLE)
				{
					mathExpressionNode_variable<mathType>* tmp = dynamic_cast<mathExpressionNode_variable<mathType>*>(this->wrapperNode->sub1()->data);
					tmp->mem->set(tmp->name,this->wrapperNode->sub2()->data->eval());
					return static_cast<double>(tmp->mem->get(tmp->name));
				}
				else throw TreeStructOops("Non variable token on lefthand side of assigment operator\n");
			}
			else
			{
				mathType arg1 = this->wrapperNode->sub1()->data->eval();
				mathType arg2 = this->wrapperNode->sub2()->data->eval();
				return this->operation(arg1, arg2);
			}
		}

	};


	template<typename mathType>
	struct mathExpressionNode_func : public mathExpressionNode
	{
		mathType (*func)(mathType);
		typedef mathType (*funcPtr)(mathType);
		mathExpressionNode_func()
		{
			this->type=FUNCTION;
		}

		mathExpressionNode_func(funcPtr operation)
		{
			this->type=FUNCTION;
			this-> func = operation;
		}

		mathType eval()
		{
			if(this->wrapperNode->sub2())
			{
					std::cerr << "Tree not correct, func type should have an empty second sub node";
			}
			return this->func(this->wrapperNode->sub1()->data->eval());
		}
	};

}
void displayToken( mathNode::mathExpressionNode* t)
{
    std::cerr << "Data type "<< t->type<< "\n";
}
#endif // MATHNODE_H_INCLUDED
