#include "mathNode.h"
namespace mathNode
{
        nodeOops::nodeOops(std::string inf)
        {
            info = inf;
        }

        const char* nodeOops::what()
        {
            return "math node Exception";
        }





		mathExpressionNode::mathExpressionNode()
		{
			type=UNKNOWN;
		}
		void mathExpressionNode::bind(node * context)
		{
			wrapperNode = context;
		}

		void mathExpressionNode::destroy() //Here goes nothing, fingers crossed
		{
			#ifdef SYNTAX_TREE_DEBUG_INFO
				std::cerr << "Delete signal received, self destruct!\n";
			#endif
			delete this;
		}




		mathExpressionNode_val::mathExpressionNode_val()
		{
			this->type=tokenType::VALUE;
		}

		mathExpressionNode_val::mathExpressionNode_val(number_type val)
		{
			value = val;
			this->type=VALUE;
		}
		number_type mathExpressionNode_val::eval()
		{
			return value;
		}










		mathExpressionNode_variable::mathExpressionNode_variable(std::string var, memory* mem)
		{
			name = var;
			this->mem = mem;
			this->type = tokenType::VARIABLE;
		}
		number_type mathExpressionNode_variable::eval()
		{
			return mem->get(name);
		}







		mathExpressionNode_opr::mathExpressionNode_opr()
		{
			this->type=OPERATOR;
			operation = nullptr;
			assign = nullptr;
			assignB = false;
		}

		mathExpressionNode_opr::mathExpressionNode_opr(operatorPtr operation)
		{
			this->type=OPERATOR;
			this->operation = operation;
			assign = nullptr;
			assignB = false;
		}
		mathExpressionNode_opr::mathExpressionNode_opr(assigmentPtr assign)
		{
			this->type = OPERATOR;
			this->operation = nullptr;
			this->assign = assign;
			assignB = true;
		}


		number_type mathExpressionNode_opr::eval()
		{
			if(this->wrapperNode->sub1() == nullptr || this->wrapperNode->sub2() == nullptr)
			{
				throw nodeOops("Panic: syntax tree has unexpected null pointer");
			}
			if (assignB)
			{
				if (this->wrapperNode->sub1()->data->type == VARIABLE)
				{
					mathExpressionNode_variable* tmp = dynamic_cast<mathExpressionNode_variable*>(this->wrapperNode->sub1()->data);
					tmp->mem->set(tmp->name,this->wrapperNode->sub2()->data->eval());
					return static_cast<double>(tmp->mem->get(tmp->name));
				}
				else throw TreeStructOops("Non variable token on lefthand side of assigment operator\n");
			}
			else
			{
				number_type arg1 = this->wrapperNode->sub1()->data->eval();
				number_type arg2 = this->wrapperNode->sub2()->data->eval();
				return this->operation(arg1, arg2);
			}
		}





        mathExpressionNode_func::mathExpressionNode_func()
		{
			this->type=FUNCTION;
		}

		mathExpressionNode_func::mathExpressionNode_func(funcPtr operation)
		{
			this->type=FUNCTION;
			this-> func = operation;
		}

		number_type mathExpressionNode_func::eval()
		{
			if(this->wrapperNode->sub2())
			{
					std::cerr << "Tree not correct, func type should have an empty second sub node";
			}
			return this->func(this->wrapperNode->sub1()->data->eval());
		}



void displayToken( mathNode::mathExpressionNode* t)
{
    std::cerr << "Data type "<< t->type<< "\n";
}
};
