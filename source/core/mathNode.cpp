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
		:value(0)
		{
			this->type=tokenType::VALUE;
		}

		mathExpressionNode_val::mathExpressionNode_val(number_type val)
		:value(val)
		{
			this->type=VALUE;
		}
		number_type mathExpressionNode_val::eval()
		{
			return value;
		}





        mathExpressionNode_variable::mathExpressionNode_variable(const mathExpressionNode_variable& val)
        {
                this->name= val.name;
                this->type= val.type;
                this->mem = val.mem;
                this->wrapperNode = val.wrapperNode;



        }

        mathExpressionNode_variable mathExpressionNode_variable::operator=(const mathExpressionNode_variable& val)
        {
                this->name= val.name;
                this->type= val.type;
                this->mem = val.mem;
                this->wrapperNode = val.wrapperNode;
                return *this;
        }



		mathExpressionNode_variable::mathExpressionNode_variable(std::string var, memory* mem)
		:mem(mem),
		name(var)

		{
			this->type = tokenType::VARIABLE;
		}
		number_type mathExpressionNode_variable::eval()
		{
			return mem->get(name);
		}







		mathExpressionNode_opr::mathExpressionNode_opr()
		:operation(nullptr),
		assign(nullptr),
		assignB(false)
		{
			this->type=OPERATOR;

		}

		mathExpressionNode_opr::mathExpressionNode_opr(operatorPtr operation)
		:operation(operation),
		assign(nullptr),
		assignB(false)
		{
			this->type=OPERATOR;
		}
		mathExpressionNode_opr::mathExpressionNode_opr(assigmentPtr assign)
		:operation(nullptr),
		assign(assign),
		assignB(true)
		{
			this->type = OPERATOR;

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
        :func(nullptr)
		{

			this->type=FUNCTION;
		}

		mathExpressionNode_func::mathExpressionNode_func(funcPtr operation)
		:func(operation)
		{
			this->type=FUNCTION;

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
