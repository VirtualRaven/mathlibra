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

		void mathExpressionNode_variable::set(number_type x)
		{
			this->mem->set(this->name, x);
		}

		mathExpressionNode_variable::mathExpressionNode_variable(std::string var, memory::memory* mem, bool b)
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
			this->ptr =nullptr;
			this->type = tree::OPERATOR;

		}

		mathExpressionNode_opr::mathExpressionNode_opr(operators::generic_oper_ptr operation)
		{
			this->ptr = operation;
			
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
			else
			{
				if (ptr != nullptr)
				{
					return this->ptr(this);
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
