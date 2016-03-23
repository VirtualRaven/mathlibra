/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "mathNode.h"
#include "tree.h"
#include "exception_helper.h"
#include "core/type_helper.h"
namespace mathNode
{

	template<EXCEPTION T> void nodeOops()
	{
		__mathlibra__raise<T,NODE>();
	}


		

		
		

		


	mathExpressionNode_val::mathExpressionNode_val()
	{
		this->type = tree::VALUE;
	}

	mathExpressionNode_val::mathExpressionNode_val(interface::type* val)
	{
		this->value  = interface::type_ptr(val);
		this->type = tree::VALUE;
	}
	mathExpressionNode_val::~mathExpressionNode_val()
	{

	}
        interface::type* mathExpressionNode_val::eval()
	{	
            return value.ptr()->copy();
	}
 	nodeDataInterface* mathExpressionNode_val::optimize()
	{
            return this;
	}
	bool mathExpressionNode_val::isPure()
	{
		return true;
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

 	bool mathExpressionNode_variable::isPure(){
		return false;
	}
	void mathExpressionNode_variable::set_mem_provider(memory::memory* mem_provider)
	{
		this->mem = mem_provider;
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

	void mathExpressionNode_variable::set(interface::type_ptr x)
	{
		this->mem->set(this->name, std::move(x));
	}

	mathExpressionNode_variable::mathExpressionNode_variable(std::string var, memory::memory* mem, bool b,math_func::function_interface* func): func(func)
	{
		this->mem = mem;
		this->_stack = b;
		this->name = var;
		this->type = tree::VARIABLE;
	}
        interface::type* mathExpressionNode_variable::eval()
	{
		return mem->get(name)->copy();
	}
	nodeDataInterface*  mathExpressionNode_variable::optimize()
	{
            return this;
	}
	bool mathExpressionNode_variable::is_pushable()
	{
		return _stack;
	}
        memory::memory* mathExpressionNode_variable::get_mem_provider()
        {
            return this->mem;
        }

        bool mathExpressionNode_variable::is_undefined()
        {
           return mem->exists(this->name);
        }
	void mathExpressionNode_variable::bind(node_base* context)
	{
		wrapperNode = context;
	}
	void  mathExpressionNode_variable::destroy()
	{
		delete this;
	}
        void mathExpressionNode_variable::makeFunction(function_obj::interpreted_func* ptr)
        {
              if (this->is_undefined() )
              {
		      nodeOops<NODE_VAR_NO_REDECLARE_FUNC>();
              }
              else if(this->func == nullptr)
              {
		      nodeOops<NODE_NO_FUNC_DEF_WITHOUT_FUNCTION_INTERFACE>();
              }
              else
              {
                auto tmp = this->func->isloaded(this->name);
                if(!tmp.loaded)
                {
                    math_func::m_function newFunc(this->name,"user","f(double)",this->name,ptr);
                    this->func->load(newFunc);
                }
                else nodeOops<NODE_NO_FUNC_REDEFINE>();
	      }

        }


	mathExpressionNode_opr::mathExpressionNode_opr()
	{
		this->ptr =nullptr;
		this->type = tree::OPERATOR;

	}
	bool mathExpressionNode_opr::isPure()
	{
		if(this->wrapperNode->sub1() == nullptr || this->wrapperNode->sub2()== nullptr)
		{
			nodeOops<NODE_UNEXPECTED_NULL_POINTER>();		
		}
		return this->wrapperNode->sub1()->data->isPure() && this->wrapperNode->sub2()->data->isPure();

	}

	mathExpressionNode_opr::mathExpressionNode_opr(operators::generic_oper_ptr operation)
	{
		this->ptr = operation;
		
		this->type = tree::OPERATOR;
	}
	
	mathExpressionNode_opr:: ~mathExpressionNode_opr()
	{

	}

        interface::type* mathExpressionNode_opr::eval()
	{
		if(this->wrapperNode->sub1() == nullptr || this->wrapperNode->sub2() == nullptr)
		{
			nodeOops<NODE_UNEXPECTED_NULL_POINTER>();
			return 0;
		}
		else
		{
			if (ptr != nullptr)
			{
				return this->ptr(this);
			}
			else
			{
				nodeOops<NODE_DUMMY_NODE_EXEC>();
				return 0;
			}

			
		}
	}
	nodeDataInterface* mathExpressionNode_opr::optimize()
	{
		if(this->wrapperNode->sub1()==nullptr || this->wrapperNode->sub2()==nullptr)
		{
			nodeOops<NODE_UNEXPECTED_NULL_POINTER>();
		}	
		if(this->isPure())
		{
			this->wrapperNode->data = new mathExpressionNode_val(this->eval());
			this->wrapperNode->data->bind(this->wrapperNode);
			static_cast<tree::node*>(this->wrapperNode)->deleteSubNodes();	
			auto tmp = this->wrapperNode->data;
                        delete this;
			return tmp;
		}	
		this->wrapperNode->sub1()->data->optimize();
		this->wrapperNode->sub2()->data->optimize();
                return this;
	}

	void mathExpressionNode_opr::bind(node_base* context)
	{
		wrapperNode = context;
	}
	void  mathExpressionNode_opr::destroy()
	{
		delete this;
	}


	
	mathExpressionNode_func_tree::mathExpressionNode_func_tree()
	{
		this->func = nullptr;
		this->type = tree::FUNCTION_TREE;
	}
	nodeDataInterface* mathExpressionNode_func_tree::optimize()
	{
		if(this->wrapperNode->sub1()==nullptr)
		{
			nodeOops<NODE_UNEXPECTED_NULL_POINTER>();
		}
		if(this->isPure())
		{
			this->wrapperNode->data = new mathExpressionNode_val(this->eval());
			this->wrapperNode->data->bind(this->wrapperNode);
			static_cast<tree::node*>(this->wrapperNode)->deleteSubNodes();	
			auto tmp = this->wrapperNode->data;
			delete this;
			return tmp;
		}	
		this->wrapperNode->sub1()->data->optimize();
                return this;
	}
	bool mathExpressionNode_func_tree::isPure()
	{
		if(this->wrapperNode->sub1() != nullptr)
		{
			return this->wrapperNode->sub1()->data->isPure();
		} 
		else  nodeOops<NODE_UNEXPECTED_NULL_POINTER>();
	}

	mathExpressionNode_func_tree::mathExpressionNode_func_tree(funcPtr operation)
	{
		this->func = operation;
		this->type = tree::FUNCTION_TREE;

	}
	mathExpressionNode_func_tree:: ~mathExpressionNode_func_tree()
	{

	}
        interface::type* mathExpressionNode_func_tree::eval()
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
          
	nodeDataInterface*  mathExpressionNode_func_user::optimize()
	{
		if(this->wrapperNode->sub1()==nullptr)
		{
			nodeOops<NODE_UNEXPECTED_NULL_POINTER>();
		}
		if(this->isPure())
		{
			this->wrapperNode->data = new mathExpressionNode_val(this->eval());
			this->wrapperNode->data->bind(this->wrapperNode);
			static_cast<tree::node*>(this->wrapperNode)->deleteSubNodes();	
                        auto tmp = this->wrapperNode->data;
			delete this;
			return tmp;
		}	
		this->wrapperNode->sub1()->data->optimize();
                return this;
	}
       	bool mathExpressionNode_func_user::isPure()
	{	
		if(this->wrapperNode->sub1()==nullptr)
		{
			nodeOops<NODE_UNEXPECTED_NULL_POINTER>();
		}
		return this->wrapperNode->sub1()->data->isPure();
	}	
        mathExpressionNode_func_user::~mathExpressionNode_func_user(){}
        mathExpressionNode_func_user::mathExpressionNode_func_user():ptr(nullptr){}
        mathExpressionNode_func_user::mathExpressionNode_func_user(function_obj::interpreted_func * ptr):ptr(ptr)
    {
        this->type = tree::tokenType::FUNCTION_USER; 
    }
                interface::type* mathExpressionNode_func_user::eval()
                {
                    if(this->ptr == nullptr)
                    {
			    nodeOops<NODE_EVAL_UNDEFINED_FUNC>();
                    }
                    if(this->wrapperNode->sub1() == nullptr )
                    {
			    nodeOops<NODE_EXPECTED_ARGUMENT_FOR_FUNC>();
                    }
                    return this->ptr->exec(this->wrapperNode->sub1()->data->eval());
                }
                void mathExpressionNode_func_user::bind(node_base* context)
                {
                    this->wrapperNode = context;
                }
                void mathExpressionNode_func_user::destroy()
                {
                    delete this;
                }
                


	    void displayToken(tree::nodeDataInterface* t)
            {
                std::cerr << "Data type "<< t->type<< "\n";
            }
}
