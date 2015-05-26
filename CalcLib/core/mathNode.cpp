#include "mathNode.h"
#include "tree.h"
namespace mathNode
{
        nodeOops::nodeOops(std::string inf, bool isCritical): exception(inf,isCritical){}

        const char* nodeOops::what()
        {
            return "math node Exception";
        }



		

		
		

		


		mathExpressionNode_val::mathExpressionNode_val()
		{
			this->value = 0;
			this->type = tree::VALUE;
		}

		mathExpressionNode_val::mathExpressionNode_val(number_type val)
		{
			this->value = val;
			this->type = tree::VALUE;
		}
		mathExpressionNode_val::~mathExpressionNode_val()
		{

		}
		number_type mathExpressionNode_val::eval()
		{
			return value;
		}
		void  mathExpressionNode_val::bind(node_base * context)
		{
			this->wrapperNode = context;
		}
		void  mathExpressionNode_val::destroy()
		{
			delete this;
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

		mathExpressionNode_variable::~mathExpressionNode_variable()
		{

		}

		mathExpressionNode_variable::mathExpressionNode_variable(std::string var, memory* mem, bool b)
		{
			this->mem = mem;
			this->_stack = b;
			this->name = var;
			this->type = tree::VARIABLE;
		}
		number_type mathExpressionNode_variable::eval()
		{
			return mem->get(name);
		}
		bool mathExpressionNode_variable::is_pushable()
		{
			return _stack;
		}


		void mathExpressionNode_variable::bind(node_base* context)
		{
			wrapperNode = context;
		}
		void  mathExpressionNode_variable::destroy()
		{
			delete this;
		}



		mathExpressionNode_opr::mathExpressionNode_opr()
		{
			this->operation =nullptr;
			this->assignB=false;
			this->type = tree::OPERATOR;

		}

		mathExpressionNode_opr::mathExpressionNode_opr(operators::operPtr operation)
		{
			this->operation = operation;
			this->assignB = false;
			this->type = tree::OPERATOR;
		}
		mathExpressionNode_opr::mathExpressionNode_opr(operators::assigmentPtr assign)
		{
			this->assign =assign;
			this->assignB=true;
			this->type = tree::OPERATOR;

		}
		mathExpressionNode_opr:: ~mathExpressionNode_opr()
		{

		}

		number_type mathExpressionNode_opr::eval()
		{
			if(this->wrapperNode->sub1() == nullptr || this->wrapperNode->sub2() == nullptr)
			{
				throw nodeOops("Panic: syntax tree has unexpected null pointer",true);
			}
			if (assignB)
			{
				if (this->wrapperNode->sub1()->data->type == tree::VARIABLE)
				{
					mathExpressionNode_variable* tmp = dynamic_cast<mathExpressionNode_variable*>(this->wrapperNode->sub1()->data);
					tmp->mem->set(tmp->name,this->wrapperNode->sub2()->data->eval());
					return static_cast<double>(tmp->mem->get(tmp->name));
				}
				else throw tree::TreeStructOops("Non variable token on lefthand side of assigment operator\n",false);
			}
			else
			{
				if (operation != nullptr)
				{
					number_type arg1 = this->wrapperNode->sub1()->data->eval();
					number_type arg2 = this->wrapperNode->sub2()->data->eval();
					return this->operation(arg1, arg2);
				}
				else
				{
					throw mathNode::nodeOops("Tried to exectue dummy node",false);
				}

				
			}
		}

		void mathExpressionNode_opr::bind(node_base* context)
		{
			wrapperNode = context;
		}
		void  mathExpressionNode_opr::destroy()
		{
			delete this;
		}



		mathExpressionNode_func::mathExpressionNode_func()
        
		{
			this->func = nullptr;
			this->type = tree::FUNCTION;
		}

		mathExpressionNode_func::mathExpressionNode_func(funcPtr operation)
		
		{
			this->func = operation;
			this->type = tree::FUNCTION;

		}
		mathExpressionNode_func::~mathExpressionNode_func()
		{

		}
		number_type mathExpressionNode_func::eval()
		{
			if(this->wrapperNode->sub2())
			{
					std::cerr << "Tree not correct, func type should have an empty second sub node";
			}
			return this->func(this->wrapperNode->sub1()->data->eval());
		}
		void mathExpressionNode_func::bind(node_base* context)
		{
			wrapperNode = context;
		}
		void  mathExpressionNode_func::destroy()
		{
			delete this;
		}
		
		mathExpressionNode_func_tree::mathExpressionNode_func_tree()
		{
			this->func = nullptr;
			this->type = tree::FUNCTION_TREE;
		}

		mathExpressionNode_func_tree::mathExpressionNode_func_tree(funcPtr operation)
		{
			this->func = operation;
			this->type = tree::FUNCTION_TREE;

		}
		mathExpressionNode_func_tree:: ~mathExpressionNode_func_tree()
		{

		}
		number_type mathExpressionNode_func_tree::eval()
		{
			if (this->wrapperNode->sub2())
			{
				std::cerr << "Tree not correct, func type should have an empty second sub node";
			}
			return this->func(static_cast<node_base*>(this->wrapperNode));
		}
		void mathExpressionNode_func_tree::bind(node_base* context)
		{
			wrapperNode = context;
		}
		void  mathExpressionNode_func_tree::destroy()
		{
			delete this;
		}

		void displayToken(tree::nodeDataInterface* t)
{
    std::cerr << "Data type "<< t->type<< "\n";
}
};
