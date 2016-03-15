/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "modules/operators.h"
#include "ptr_protect.h"
#include "core/internal_helper.h"
#include "core/function_obj.h"
#include "core/mathNode.h"
#include "exception_helper.h"
#include "core/type.h"
#include "core/type_helper.h"
namespace operators
{

   	template<EXCEPTION T> void stdOperatorOops()
	{
		__mathlibra__raise<T,STD_OPERATORS>();
	}
	type* add(double  x, double y)
	{
		return make_type(x+y);
                
	}
	type* __add(tree::nodeDataInterface*n)
	{
		return internal_helper::forward_fast<double,double>(add, n);
	}

        type* multi(num_mat x, num_mat y)
	{
		if(x.isNumber() && y.isNumber())
		{
			*x.begin() = (*x.begin())*(*y.begin());
			return x.copy();
		}
		else if(x.isNumber())
		{
			double oper = *x.begin();
			for(auto it = y.begin(); it < y.end(); it++)
			{
				*it=oper*(*it);
			}
			return y.copy();
		}
		else if(y.isNumber())
		{
			return multi(std::move(y),std::move(x));
		}
		//Preform matrix multiplication
		else if(x.sizeM() == y.sizeN())
		{
			//The return value must be an pointer, thus we allocate the 
			//result matrice on the heap
			num_mat*  res = new num_mat(x.sizeN(),y.sizeM());
			auto itr = res->begin();
			auto itx = x.begin();
			for(size_t i=0; i < x.sizeN(); i++)
			{
				auto ity = y.begin<true>();
				for(size_t j=0; j < y.sizeM(); j++)
				{
					double sum=0;
					for(size_t k=0; k < y.sizeN(); k++)
					{
						sum += (*(ity+k)) * (*(itx+k));
					}
					*itr=sum;
					++itr;
					//Move ity to begining of next column
					ity+=y.sizeN();
				}
				//Move itx to begining of next row
				itx+=x.sizeM();
			}
			return res;

		}
		stdOperatorOops<MUL_OPR_INVALID_ARGS>();
		
	}
	type* __multi(tree::nodeDataInterface* n)
	{
		return internal_helper::forward_fast<num_mat, num_mat>(multi, n);
	}
	type* divide(double x, double y)
	{
		return make_type(x/y);
	}
        type* declare_func(tree::nodeDataInterface* n)
        {
            tree::node*  node = static_cast<tree::node*>(nodeDataInterface_wrapper_access(n));
            
            if(node->sub1() != nullptr  )
            {
                auto type = node->sub1()->data->type;
                if(type == tree::VARIABLE)
                { 
                    mathNode::mathExpressionNode_variable* var = static_cast<mathNode::mathExpressionNode_variable*>(node->sub1()->data);
                    ptr_protect<tree::node*,false> expr(node->take_ownership_sub2());
                    if(expr.ptr() != nullptr)
                    {
                        function_obj::interpreted_func* func= new function_obj::interpreted_func(expr.ptr(),var->get_mem_provider());
                        var->makeFunction(func);  
                        expr.release();   
                        return make_type(1);
                    }
                    else 
                    {
			    stdOperatorOops<FUNC_OPR_NEED_EXPR>();
                    }
                }
                else if(type == tree::FUNCTION_USER )
                {
		    stdOperatorOops<FUNC_OPR_NO_REDEFINE>();
                }
                else if(type == tree::FUNCTION || type == tree::FUNCTION_TREE)
                {
		    stdOperatorOops<FUNC_OPR_NO_REDEFINE_SYSTEM>();
                }

            }
            else
            {
		stdOperatorOops<FUNC_OPR_NEED_FUNC_NAME>();
            } 
            
            return make_type(0);     
        }

        type* __divide(tree::nodeDataInterface* n)
	{
		return internal_helper::forward_fast<number_type, number_type>(divide, n);
	}
	type* sub(number_type x, number_type y)
	{
		return make_type(x-y);
	}
	type* __sub(tree::nodeDataInterface* n)
	{
		return internal_helper::forward_fast<number_type, number_type>(sub, n);
	}
	type* __pow(tree::nodeDataInterface* n)
	{
		return internal_helper::forward_fast<number_type,number_type>( pow,n);
	}

	type* assign(mathNode::mathExpressionNode_variable_interface* var, interface::type_ptr val)
	{
		auto tmp = val->copy();
		var->set(std::move(val));

		return tmp;
	}

	type* __assign(tree::nodeDataInterface* n)
	{
		return internal_helper::forward_fast<mathNode::mathExpressionNode_variable_interface*, interface::type_ptr>(assign, n);
	}
	
	
	type* dummy(tree::nodeDataInterface* )
	{
		return nullptr;
	}

    vec_operators std_operators= {
        interpreter_operator(&__sub,'-',1),
		interpreter_operator(&__add,'+' ,1),
		interpreter_operator(&__multi,'*' ,2),
		interpreter_operator(&__divide,'/' ,2),
		interpreter_operator(&__pow,'^' ,3),
        interpreter_operator(&__assign,'=',0),
		interpreter_operator((generic_oper_ptr)nullptr, ',', 0 ),
                interpreter_operator(&declare_func,':',0)
    };

operators::interpreter_operator::interpreter_operator(generic_oper_ptr opr, char symbol, short wheight):
  ptr(opr),
  operChar(symbol),
  baseWheight(wheight)
{}

operators::interpreter_operator::interpreter_operator():
 ptr(nullptr),
 operChar(0),
 baseWheight(0)
 {}




        void operators_interface::load(vec_operators oprs)
        {
            this->_operators.insert(_operators.end(),oprs.begin(),oprs.end());
        }
        void operators_interface::load(interpreter_operator tok)
        {
            this->_operators.push_back(tok);
        }

operators_interface::operators_interface()
: _operators(std_operators),
lastOffset(0)

{}

	bool  operators_interface::inArray(char opr)
	{
		for(unsigned int i =0; i< this->_operators.size(); i++)
		{
			if(this->_operators[i].operChar == opr)
			{
				lastOffset =i;
				return true;
			}
		}
		return false;
	}
	interpreter_operator  operators_interface::getCurrent()
	{
		return this->_operators[lastOffset];
	}


}
