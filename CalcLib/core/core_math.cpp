#include "core_math.h"
#include <cmath>
#include "core/mathNode.h"
#include "modules/function_helper.h"
#include <vector>
#include <stack>






namespace core_math
{
	coreMathOops::coreMathOops(std::string inf)
	{
		info = inf;
	}
	const char* coreMathOops::what()
	{
		return "Core math exception";
	}

	typedef unsigned long long u_big_int;

	u_big_int  integerReqiured(double arg) // Throws exception if not convertable to integer
	{
		if ((fabs(round(arg) - arg) <= 0.00001f)) //Check if value is an integer within given bounds
		{
			return (u_big_int)arg;
		}
		else
		{
			throw math_func::functionOops("invalid argument, excpected integer");
		}
	}

	double frac(double arg) 
	{
		u_big_int argI = integerReqiured(arg);
			
			std::cout << "{ ";
			for (u_big_int i = 2; i <= floor(sqrt(argI)); i++)
			{
				if (argI % i == 0) // if true then i is a factor of argI
				{
					std::cout   << i << ", ";
					argI = argI / i;
					i = 1;
					continue;
				}
			}
			std::cout << argI << " }";
			return argI;
		
		
		return 0;
	}

	

	double factorial(double arg)
	{
		u_big_int argI = integerReqiured(arg);
		for (u_big_int i = argI - 1; i > 1; i--)
		{
			argI = argI*i;
		}
		return argI;
	}

	
	struct breadth_traversal_unit
	{
		node * n;
		int i;
		breadth_traversal_unit(node * node, int index) : n(node), i(index) {}
	};
	
	
	double nPr(double x, double y)
	{
		
		return factorial(x) / factorial(x - y);

	}

	double nCr(double x, double y)
	{
		return nPr(x, y) / factorial(y);
	}

	double nPr_wrapper(node * n)
	{
		return function_helper::forward<double, double>(nPr, n);
	}

	double nCr_wrapper(node *n)
	{
		
		return function_helper::forward<double, double>(nCr, n);
		
	}
	
	double _d_displayTree(node * n)
	{
		

		std::vector<std::vector<breadth_traversal_unit>> pvec;
		std::vector<breadth_traversal_unit> vec;
		vec.push_back(breadth_traversal_unit(n,1));
		pvec.push_back(vec);
		int level=0;
		do
		{
			
			std::vector<breadth_traversal_unit> lvec;
			int index=1;
			for each (breadth_traversal_unit var in pvec[level])
			{
				if (var.n->sub1() != nullptr)
				{
					lvec.push_back(breadth_traversal_unit(var.n->sub1(), index));
				}
				index++;
				if (var.n->sub2() != nullptr)
				{
					lvec.push_back(breadth_traversal_unit(var.n->sub2(), index));
				}
				index++;
			}

			if (lvec.size() < 1)
			{
				break;
			}

			level++;
			pvec.push_back(lvec);
			
		} while (true);
		
		int row=1;

		for each (std::vector<breadth_traversal_unit> breadthvec in pvec)
		{
			std::cout << "Row" << row;
			for each(breadth_traversal_unit current in breadthvec)
			{
				switch (current.n->data->type)
				{
				
				case tree::VARIABLE:
				{
					mathNode::mathExpressionNode_variable* tmp = dynamic_cast<mathNode::mathExpressionNode_variable*>(current.n->data);
					std::cout << "{Variable, " << tmp->name << "} ";
				}break;
				case tree::FUNCTION:
				{
					mathNode::mathExpressionNode_func* tmp = dynamic_cast<mathNode::mathExpressionNode_func*>(current.n->data);
					std::cout << "{Function, " << (void*)tmp->func << "} ";
				}break;
				case tree::OPERATOR:
				{
					mathNode::mathExpressionNode_opr* tmp = dynamic_cast<mathNode::mathExpressionNode_opr*>(current.n->data);
					std::cout << "{Operator, " << (void*)tmp->operation << "} ";
				}break;
				case tree::VALUE:
				{
					mathNode::mathExpressionNode_val* tmp = dynamic_cast<mathNode::mathExpressionNode_val*>(current.n->data);
					std::cout << "{Value, " << tmp->value << "} ";
				}break;
				case tree::FUNCTION_TREE:
				{
					mathNode::mathExpressionNode_func_tree* tmp = dynamic_cast<mathNode::mathExpressionNode_func_tree*>(current.n->data);
					std::cout << "{Function tree, " << (void*)tmp->func << "} ";
				}break;

				case tree::DUMMY:
				{
					std::cout << "{Dummy node} ";
				}break;
				default:
					std::cout << "Failed to display tree\n";
					return 0;
					break;
				}
			}
			std::cout << "\n";
			row++;
		}

		return 1;
	}

	


	std::vector<math_func::m_function> lib_core_math =
	{
		math_func::m_function("frac", static_cast<double(*)(double)>(frac)),
		math_func::m_function("factorial", static_cast<double(*)(double)>(factorial)),
		math_func::m_function("displayTree", static_cast<double(*)(node*)>(_d_displayTree)),
		math_func::m_function("nPr", static_cast<double(*)(node*)>(nPr_wrapper)),
		math_func::m_function("nCr", static_cast<double(*)(node*)>(nCr_wrapper))
	};
};