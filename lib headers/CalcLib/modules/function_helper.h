#ifndef FUNCTION_HELPER_INCLUDED
#define FUNCTION__HELPER_INCUDED
#include <stack>
#include "core/mathNode.h"

namespace parameter_package
{


	template<typename... argN>
	class package{};

	template<typename arg0, typename... argN>
	class package<arg0, argN...> : public package<argN...>
	{
	public:
		package(arg0 arg, argN... args) : tail(arg), package<argN...>(args...){};
		package() : package<argN...>(){};
		package(arg0 arg, package<argN...> pack) : tail(arg), package<argN...>(pack){};
		arg0 tail;

	};

	template<typename arg0>
	class package<arg0>
	{
	public:
		package(arg0 arg) : tail(arg){};
		package() {};
		arg0 tail;

	};

	template<typename Return_value, typename Func, typename arg0, typename ... argN, typename... argN2>
	Return_value package_forward(Func func, package<arg0, argN...> pack, argN2... args)
	{
		return package_forward<Return_value>(func, (package<argN...>)pack, args..., pack.tail);
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

namespace function_helper
{
	using tree::node_base;
	//Breadth first function for displaying the syntax tree under the node_base
	std::stack<tree::node_base*> getArgs(node_base * n)
	{
		node_base * current = n;
		node_base * next = nullptr;
		std::stack<node_base*> args;

		while (current != nullptr)
		{
			//Check second branch 
			if (current->sub2() != nullptr)
			{
				if (current->sub2()->data->type == tree::DUMMY)
				{
					next = current->sub2();
				}
				else
				{
					args.push(current->sub2());
				}
			}

			//Check first branch
			if (current->sub1() != nullptr)
			{
				if (current->sub1()->data->type == tree::DUMMY)
				{
					next = current->sub1();
				}
				else
				{
					args.push(current->sub1());
				}
			}

			current = next;
			next = nullptr;
		}
		return args;
	}

	template<typename T> T getData(node_base *n)
	{
		if (n->data->type == mathnode::helper::enum_type<T>::TYPE)
		{
			return static_cast<T>(n->data);
		}
		else
		{
			throw core_math::coreMathOops("Wrong argument type");
		}
	}
	template<> double getData<double>(node_base * n)
	{
		return n->data->eval();
	}


	template <typename... argN> struct func_type
	{
		typename typedef double(*type)(argN...);
	};

	template< typename... argN> double forward(typename func_type<argN...>::type  func, node_base * n)
	{
		auto args = getArgs(n);
		if (args.size() != sizeof...(argN))
		{
			throw core_math::coreMathOops("Function called with wrong number of argumets",false);
		}

		parameter_package::package<argN...> pack = fillPackage<argN...>(args);
		return parameter_package::package_forward<double, typename func_type<argN...>::type>(func, pack);
	}



	template< typename arg0, typename arg1, typename... argN> auto fillPackage(std::stack<node_base*>& s) -> parameter_package::package<arg0, arg1, argN...>
	{
		auto tmp = getData<typename arg0>(s.top());
		s.pop();
		return parameter_package::package<arg0, arg1, argN...>(tmp, fillPackage<arg1, argN...>(s));

	}
	template< typename arg0> auto  fillPackage(std::stack<node_base*>& s) -> parameter_package::package<arg0>
	{
		auto tmp = getData<arg0>(s.top());
		s.pop();
		return parameter_package::package<arg0>(tmp);
	};

}

#endif