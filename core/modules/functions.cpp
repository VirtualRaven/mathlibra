/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "modules/functions.h"
#include "exception_helper.h"
#include "core/internal_helper.h"
#include <algorithm>
//Contains temporary implementation of function module
namespace math_func
{
	template<EXCEPTION T> void functionOops()
	{
		__mathlibra__raise<T,FUNCTION>();
	}

	typedef double(*double_func)(double);


	math_func::m_function::m_function(std::string name,std::string tag,std::string doc,std::string disp_name, generalFuncPtr ptr) : type(func_type::GENERAL), 
				name(name), 
				tag(tag),
				doc(doc),
				disp_name(disp_name),
				gptr(ptr) {}

	math_func::m_function::~m_function(){}

	math_func::m_function::m_function() :type(func_type::GENERAL),
					     name(), 
					     tag(),
					     doc(),
					     disp_name(),
					     gptr(nullptr){}
        math_func::m_function::m_function(std::string name, std::string tag,std::string doc,std::string disp_name,interpreted_func* ptr): type(func_type::USER),
       				  name(name),
				  tag(tag),
			       	  doc(doc),
			       	  disp_name(disp_name),
				  uptr(ptr) {}

		void function_interface::load(std::vector< m_function>& obj)
		{
			funcs.insert(funcs.end(),obj.begin(),obj.end());
		}
		void function_interface::load(m_function obj)
		{
			funcs.push_back(obj);
		}
		load_test_return  function_interface::isloaded(std::string funcName)
		{
			for (unsigned int i = 0; i < funcs.size(); i++)
			{
				if (funcs[i].name == funcName)
				{
					cache = funcs[i];
					return {true, cache.type == func_type::USER };
				}
			}
			return {false,false};
		}
		
		func_type function_interface::type()
		{
			return this->cache.type;
		}
		u_ptr function_interface::get(std::string funcName)
		{
			if (cache.name == funcName)
			{
				if(cache.type == func_type::USER)
                                {
					functionOops<FUNC_NO_CONV_FUNCTOR_VOID_P>();
                                }
                                return reinterpret_cast<u_ptr>(cache.gptr);
			}
			else
			{

				for (unsigned int i = 0; i < funcs.size(); i++)
				{
					if (funcs[i].name == funcName)
					{
                                            if(funcs[i].type == func_type::USER)
                                            {
						functionOops<FUNC_NO_CONV_FUNCTOR_VOID_P>();
                                            }
					    else return reinterpret_cast<u_ptr>(funcs[i].gptr);
					}
				}
				return nullptr;
			}
		}
                
                interpreted_func* function_interface::getFunctor(std::string funcName)
                {
                    if(cache.name ==funcName && cache.type == USER)
                    {
                            return cache.uptr;
                    }
                    else
                    {
                        for(unsigned int i =0; i < funcs.size(); i++)
                        {
                            if(funcs[i].name == funcName)
                            {
                                return cache.uptr;
                            }
                        }
                        return nullptr;
                    }   
                }
                void function_interface::display()
		{
			std::cout << "-[ function_interface {\n";
			for (unsigned int i = 0; i < funcs.size(); i++)
			{
				std::cout << this->funcs[i].name.c_str() << "\t: " <<  this->funcs[i].tag   << "\n";
			}
			std::cout << " }\nLoaded " << this->funcs.size() << " functions]\n";

		}
                function_interface::~function_interface()
                {
                        for(unsigned int i =0; i < funcs.size(); i++)
                        {
                            if(funcs[i].type == func_type::USER)
                            {
                                delete funcs[i].uptr;
                            }
                        }
                }
		std::vector<std::string> function_interface::getFunctionNames()
		{
			std::vector<std::string> vec;
			vec.reserve(this->funcs.size());
			for (unsigned int i = 0; i < funcs.size(); i++)
			{
				vec.push_back(this->funcs[i].name);
			}
			return vec;
		}
	
		std::vector<func_obj_info> function_interface::getFunctionObjs()
		{
			std::vector<func_obj_info> vec;
			vec.reserve(this->funcs.size());
			for (unsigned int i = 0; i < funcs.size(); i++)
			{
				vec.push_back( {
						this->funcs[i].name,
						this->funcs[i].tag,
						this->funcs[i].doc,
						this->funcs[i].disp_name
						});
			}
			return vec;


		}
		void function_interface::unload(std::string name)
		{
			for(auto it = funcs.begin(); it< funcs.end(); it++)
			{
				if(it->name==name)
				{
					funcs.erase(it);	
					return;
				}
			}
		}
                type* iterate(tree::nodeDataInterface* next, 
                                mathNode::mathExpressionNode_variable_interface* var,
                                num_mat start,
                                double iterations)
                {
                    //As we are going to iterate over next it is best to take the time and
                    //optimize it
                    next=next->optimize();
                    var->set(interface::type_ptr(start.copy())); 
                    for(double i=0; i < iterations; i++)
                    {
                        var->set(interface::type_ptr(next->eval()));    
                    }
                    return var->eval();
                }
                type* __iterate(node_base* b)
                {
                    return function_helper::forward<tree::nodeDataInterface*,
                                                    mathNode::mathExpressionNode_variable_interface*,
                                                    num_mat,
                                                    double>(iterate,b);
                }
                type* __sin(node_base* b)
                {
                    return internal_helper::forward_fast<double>(sin,b); 
                }
                type*  __cos(node_base *b)
                {
                    return internal_helper::forward_fast<double>(cos,b);
                }
                type*  __tan(node_base *b)
                {
                    return internal_helper::forward_fast<double>(tan,b);
                }

                type* __asin(node_base *b)
                {
                    return internal_helper::forward_fast<double>(asin,b);
                }
                type*  __acos(node_base *b)
                {
                    return internal_helper::forward_fast<double>(acos,b);
                }
                type* __atan(node_base *b)
                {
                    return internal_helper::forward_fast<double>(atan,b);
                }
                type*  __sqrt(node_base *b)
                {
                    return internal_helper::forward_fast<double>(sqrt,b);
                }
                type*  __log(node_base *b)
                {
                    return internal_helper::forward_fast<double>(log,b);
                }
                type* __log10(node_base *b)
                {
                    return internal_helper::forward_fast<double>(log10,b);
                }

                type* __ceil(node_base *b)
                {
                    return internal_helper::forward_fast<double>(ceil,b);
                }

                type* __floor(node_base *b)
                {
                    return internal_helper::forward_fast<double>(floor,b);
                }
                
                type* __round(node_base *b)
                {
                    return internal_helper::forward_fast<double>(round,b);
                }
		type* size(interface::type_ptr x)
		{
			auto tmp = new num_mat(1,2);
			auto it = tmp->begin();
			*it++=x->sizeN();
			*it=x->sizeM();
			return tmp;
		}	
		type* __size(node_base* n)
		{
			return internal_helper::forward_fast<interface::type_ptr>(&size,n);
		}
		template<bool B> size_t choose(size_t x, size_t)
		{
			return x;
		}
		template<> size_t choose<false>(size_t, size_t y)
		{
			return y;
		}
		template<bool B>	
		type* num_matrix_constructor(node_base *b)
		{
			std::stack<tree::node_base*>  args= b->getArgs();
			if(args.empty())
			{
				functionOops<EMPTY_MAT_INIT>();
			}
			auto tmp = interface::type_ptr(args.top()->data->eval());
			size_t args_len= args.size();
			args.pop();
			switch (tmp->stores())
			{
				case T_DOUBLE:
				{
					std::vector<interface::type_ptr> vec;
					vec.reserve(args_len);
					
					auto n = tmp->sizeN();
					auto m = tmp->sizeM();
					auto totalM = choose<B>(m,n);
					vec.push_back(std::move(tmp));
					for(size_t i=0; i< args_len-1; i++ )
					{
						auto tmp2 = interface::type_ptr(args.top()->data->eval());
						auto n2 = tmp2->sizeN();
						auto m2 = tmp2->sizeM();
						args.pop();
						if(choose<B>(n2,m2) != choose<B>(n,m))
						{
							functionOops<DIFFERENT_N_MAT_INIT>();	
						}
						else if(tmp2->stores() != T_DOUBLE)
						{
							functionOops<DOUBLE_REQUIRED_MAT_INIT>();		
						}
						else
						{
							totalM += choose<B>(m2,n2);
							vec.push_back(std::move(tmp2));
						}
					}
					num_mat* mat = new num_mat(choose<B>(n,totalM),choose<B>(totalM,m));
					auto it = mat->begin<B>();
					for(size_t i=0; i<args_len; i++)
					{
					      it= std::copy(static_cast<num_mat*>(vec[i].ptr())->begin<B>(),	
							static_cast<num_mat*>(vec[i].ptr())->end<B>(),
							it);
					}			
					return mat;
				}break;

				default:
					functionOops<MATRICE_MUST_BE_DOUBLE_MAT_INIT>();
			}
			return nullptr;
		}
		 std::vector< math_func::m_function> std_math_trig_func = {
			 math_func::m_function("sin","trig","sin(double), standard sinus functions","sin", __sin),
			 math_func::m_function("cos","trig","cos(double), standard cosinus function","cos", __cos),
			 math_func::m_function("tan","trig","tan(double), standard tan function","tan", __tan),
			 math_func::m_function("asin","trig","asindouble), inverse sin function","asin", __asin),
			 math_func::m_function("acos","trig","acos(double), inverse cosinus function","acos", __acos),
			 math_func::m_function("atan","trig","stan(double), inverse tan function","atan", __atan)
		};

		std::vector<math_func::m_function> std_math_func = {
			//math_func::m_function("abs", static_cast<double(*)(double)>(abs)), //Only defined with double overload on certain platforms
			math_func::m_function("sqrt","numeric","sqrt(double), square root function function","\u221A", __sqrt),
			math_func::m_function("ln","numeric","ln(double), natural logarithm function","ln", __log),
			math_func::m_function("log","numeric","log(double),  logarithm base ten","log", __log10)
		};

		std::vector<math_func::m_function> std_math_num_func = {
			math_func::m_function("ceil","numeric","ceil(double)","ceil", __ceil),
			math_func::m_function("floor","numeric","floor(double)","floor", __floor),
			math_func::m_function("round","numeric","round(double)","round", __round),
                        math_func::m_function("it","numeric","it(expr,variable,num_mat,double)","it", __iterate)
		};
		
		std::vector<math_func::m_function> mathlibra_data_constructors = {
			math_func::m_function("mat","built_in","mat(mat_double...)","mat",&num_matrix_constructor<true>),
			math_func::m_function("matc","built_in","matc(mat_double...)","matc",&num_matrix_constructor<false>),
			math_func::m_function("size","built_in","size(mat)","size",__size)
		};	
}

/*
bool test::function_module_test1()
{
	math_func::function_interface func;
	func.load(math_func::std_math_trig_func);
	func.load(math_func::m_function("gen","","","", (math_func::m_function::generalFuncPtr)(nullptr)));
	if (!(func.isloaded("sin").loaded && func.isloaded("cos").loaded && func.isloaded("tan").loaded && func.isloaded("gen").loaded))
	{
		return false;
	}
	if ((math_func::m_function::generalFuncPtr)func.get("gen") != nullptr || func.get("sin") != reinterpret_cast<math_func::u_ptr>(sin))
	{
		return false;
	}
	func.isloaded("gen");
	if (func.type()  != math_func::GENERAL)
	{
		return false;
	}
	return false;
}

*/
