/**
 * Defines helper templates to convert node_base pointers to function arguments.
 */
#ifndef FUNCTION_HELPER_INCLUDED
#define FUNCTION_HELPER_INCUDED
#include <stack>
#include "core/mathNode_interface.h"
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
			n->raiseException("Wrong argument type");
			return nullptr;
		}
	}
	template<> nodeDataInterface* getData<nodeDataInterface*>(node_base * n)
	{
		return n->data;
	}
	template<> double getData<double>(node_base * n)
	{
		return n->data->eval();
	}


	template <typename... argN> struct func_type
	{
		 typedef double(*type)(argN...);
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
	/**
	 * 
	 * @function Forward function call. 
	 * If the arguments required for the call can not be exctracted from the node_base an exception will be raised in the core.
	 * @param func An pointer to an function of the type double f(vars...) where vars is an variable number of arguments
	 * @param n Pointer to the node to use to extract requested data.
	 * @tparam argN An list of the arguments  accepted by func.
	 * @note Any exceptions thrown by func will be caought an automatically forwarded to the core, so feel free to throw any exception.
	 * 
	 *
	 */
	
	template< typename... argN> double forward(typename func_type<argN...>::type  func, node_base * n)
	{
		auto args = n->getArgs();
		if (args.size() != sizeof...(argN))
		{
			n->raiseException("Function called with wrong number of argumets");
		}

		parameter_package::package<argN...> pack = fillPackage<argN...>(args);
		try
		{
			return parameter_package::package_forward<double, typename func_type<argN...>::type>(func, pack);
		}
		catch (std::exception& e)
		{
			n->raiseException(e.what());
		}
		catch (...)
		{
			n->raiseException("Unknown exception occured in plugin function");
		}
		return 0;

	}



}

#endif
