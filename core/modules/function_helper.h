/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/**
 * Defines helper templates to convert node_base pointers to function arguments.
 */
#ifndef FUNCTION_HELPER_INCLUDED
#define FUNCTION_HELPER_INCLUDED
#include <stack>
#include "core/mathNode_interface.h"
#include "recursion_types.h"
#include "core/type.h"
#include <iostream>
/**
* @namespace parameter_package Functions for creating packages of function arguments of different types
*/ 
namespace parameter_package
{

	
	template<typename... argN>
	class package{};

	template<typename arg0, typename... argN>
	class package<arg0, argN...> : public package<argN...>
	{
	public:
		arg0 tail;
		package(arg0 arg, argN... args) : tail(arg), package<argN...>(args...){};
		package() : package<argN...>(){};
		package(arg0 arg, package<argN...> pack) :  package<argN...>(pack), tail(arg){};


	};

	template<typename arg0>
	class package<arg0>
	{
	public:
		package(arg0 arg) : tail(arg){};
		package() {};
		arg0 tail;

	};

	template<typename Return_value, typename Func, typename arg0, typename arg1, typename ... argN, typename... argN2>
	Return_value package_forward(Func func, package<arg0,arg1, argN...> pack, argN2... args)
	{
		return package_forward<Return_value>(func, (package<arg1, argN...>)pack, args..., pack.tail);
	}

	template<typename Return_value, typename Func, typename arg0, typename... argN2>
	Return_value package_forward(Func func, package<arg0> pack, argN2... args)
	{
		return func(args..., pack.tail);
	}

	template<typename arg0, typename... argN>
	void display_package(package<arg0, argN...> arg)
	{
		std::cout << arg.tail << " ";
		display_package<argN...>(arg);
	}

	template<typename arg0>
	void display_package(package<arg0> arg)
	{
		std::cout << arg.tail << std::endl;
	}

}
/**
 * @namespace function_helper Contains functions automatic function forwarding.
 */
namespace function_helper
{
        class f_exception : public std::exception
        {
            std::string _str;
            public:
            f_exception(std::string e):_str(e) {}
            virtual const char* what() const noexcept
            {
                return _str.c_str();
            }
        };

	using tree::node_base;

	/** 
	 * Extract data of Type T from the node_base.
	 * getData tries to interpret the data in the node as an type T, if unsuccessful an exception is raised.
	 * @param n Pointer to the node to extract data from.
	 * @tparam T, the type to interpret the data as. It can be an double or any non abstract type inheriting for nodeDataInterface.
	 */
	template<typename T> T getData(node_base *n)
	{
		if (n->data->type == mathNode::helper::enum_type<T>::TYPE)
		{
			return static_cast<T>(n->data);
		}
		else
		{
			throw f_exception("Wrong argument type, expected");
		}
	}
        /**
	 * Tries to extract the requested type T from the tree n 
	 * by evaluating the tree and checking the returned type object.
	 * If the this object matches the requested type the object is returned,
	 * else an exception is raised in mathlibra.
	 * @param n Pointer to the node to extract data from.
	 * @tparam T, the requested type. 
	 */ 
        template<typename T> T convertData(node_base* n )
        {
            auto tmp = n->data->eval();
           if(tmp->stores() == get_enum<T>::t() )
           {
                auto tmp2 = T(*static_cast<T*>(tmp)); 
                n->free_type(tmp);
                return tmp2;
           }
           else
           {
                n->free_type(tmp);
                throw f_exception("Wrong argument type, expected");
           } 
        }

	template<> inline nodeDataInterface* getData<nodeDataInterface*>(node_base * n)
	{
		return n->data;
	}
         
        template<> inline mat_mat getData<mat_mat>(node_base *n)
        {
             return convertData<mat_mat>(n); 
        }
        template<> inline char_mat getData<char_mat>(node_base *n)
        {
            return convertData<char_mat>(n);
        }
        template<> inline num_mat getData<num_mat>(node_base* n)
        {
            return convertData<num_mat>(n);
        }
		template<> inline interface::type_ptr getData<interface::type_ptr>(node_base* n)
		{
			return interface::type_ptr(n->data->eval(), n->get_free_func());
		}
	
	template<> inline double getData<double>(node_base * n)
	{
	    auto tmp = n->data->eval();
            double res=0;
            if(tmp->isNumber())
            {
                res=tmp->toNumber();   
                n->free_type(tmp);
                return res;
            }
            else
            {   
                n->free_type(tmp);
                throw f_exception("Invalid argument, expected");
            }
	}
	/**
	 *Template struct defining function signatures.
	 * The struct contains f_type which is an typedef of 
	 * a function returning a double an having parameter arg0, arg1 ... argN.
	 */
        template <typename... argN> struct func_type_double
        {
            typedef double(*f_type)(argN...);
        };
	template <typename... argN> struct func_type
	{
		 typedef interface::type*(*f_type)(argN...);
	};
/**
 * Fills the and parameter package with data. It uses the getData() function to exctract data from the nodes an fills the parameter_package wiith it.
 * @param s An stack of node_base* to use to fill the parameter_package.
 */
template< typename arg0> auto  fillPackage(std::stack<node_base*>& s) -> parameter_package::package<arg0>
	{
		auto tmp = getData<arg0>(s.top());
		s.pop();
		return parameter_package::package<arg0>(tmp);
	}

	template< typename arg0, typename arg1, typename... argN> auto fillPackage(std::stack<node_base*>& s) -> parameter_package::package<arg0, arg1, argN...>
	{
		auto tmp = getData<arg0>(s.top());
		s.pop();
		return parameter_package::package<arg0, arg1, argN...>(tmp, fillPackage< arg1, argN...>(s));

	}

	template<typename T> inline std::string signature()
	{
		return "UNKNOWN";
	}
	template<> inline std::string signature<double>()
	{
		return "number";
	}
	template<> inline std::string signature<num_mat>()
	{
		return "matrix";
	}	
	template<> inline std::string signature<char_mat>()
	{
		return "string";
	}
	template<> inline std::string signature<mat_mat>()
	{
		return "list";
	}
	template<> inline std::string signature<nodeDataInterface*>()
	{
		return "generic";
	}
	template<> inline std::string signature<mathNode::mathExpressionNode_variable_interface*>()
	{
		return "variable";
	}
	template<> inline std::string signature<interface::type_ptr>()
	{
		return "generic";
	}

	template<typename arg0> std::string create_signature_string()
	{
		return signature<arg0>();
	}
	template<typename arg0,typename arg1, typename... argN>  std::string create_signature_string()
	{
		return signature<arg0>()  + "," + create_signature_string<arg1,argN...>();	
	}
        
	template<typename arg0> std::string create_signature_string2(recursion::array_slice<1,std::string> n)
	{
		return signature<arg0>() + " " + n.head();
	}
	template<typename arg0,typename arg1, typename... argN>  std::string create_signature_string2(recursion::array_slice<2+sizeof...(argN),std::string> n)
	{
		return signature<arg0>() + " " + n.head()  + "," + create_signature_string2<arg1,argN...>(n.tail());	
	}
        
        

	/**
	 * Forward function.
	 * This function takes an node_base* which is what mathlibra passes to an function when it is evaluated, it then converts it
	 * into the arguments needed for a function call to the provided function.
	 * If the arguments required for the call can not be exctracted from the node_base, an exception will be raised in mathlibra.
	 * Forward has two main modes of operation, evaluation and convertion. If you request a double forward will fullfil this request by forcing evaluation of the syntax tree 
	 * and the it then converts the resulting value into the requested one if possible.
	 * If you instead request an syntax node type, for example mathNode::mathExpressionNode_variable_interface (which represents an mathlibra variable) the tree is not evaluated,
	 * instead forward checks if n can be safely convertet into such a variable.  This behavior can be exploited to get great controll over the expression execution.
	 * Currently the forward function can create arguments of type:
	 * 	- double
	 * 	- char_mat
	 * 	- num_mat
	 * 	- mat_mat
	 * 	- type_ptr
	 * 	- mathNode types
	 *
	 * If you define an function type* f(double,double,num_mat); the you call forward by forward<double,double,num_mat>(f,n);
	 * @param func An pointer to an function of the type double f(vars...) where vars is an variable number of arguments
	 * @param n Pointer to the node in the syntax tree 
	 * @tparam argN An list of the arguments  accepted by func.
	 * @note Any exceptions thrown by func will be caught and automatically forwarded to the to mathlibra If the exception is .
	 * @note The return value of the function to be forwarded is automatically transfered over library boundries and safely deleted when no longer needed. thus statements like return new type(); is
	 * 	valid inside any function managed by the forward function.
	 */
	template< typename... argN> type* forward(typename func_type<argN...>::f_type  func, node_base * n,recursion::fixed_array<sizeof...(argN),std::string>* names )
	{
		if(n==nullptr)
		{
                    std::string tmp;
                    if(names)
			tmp = create_signature_string2<argN...>(names->tail());
                    else
                        tmp = create_signature_string<argN...>();

			return new char_mat(tmp.c_str(),1,tmp.size());
		}
		auto args = n->getArgs();
		try
		{
			if (args.size() != sizeof...(argN))
			{
				throw f_exception("Function called with wrong number of argumets, expected");
			}

		        parameter_package::package<argN...> pack = fillPackage<argN...>(args);
			auto tmp =parameter_package::package_forward<type*, typename func_type<argN...>::f_type>(func, pack);
                        return n->realloc(tmp);
		}
		catch (std::exception& e)
		{
			n->raiseException( (std::string(e.what()) + " (" + create_signature_string<argN...>()+ ")").c_str() );
		}
		catch (...)
		{
			n->raiseException("Unknown exception occured in plugin function");
		}
		return 0;

	}
	template< typename... argN> type* forward(typename func_type<argN...>::f_type  func, node_base * n)
	{
            return forward<argN...>(func,n,0);
	}

}

#endif
