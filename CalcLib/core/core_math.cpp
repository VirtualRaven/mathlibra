#include "core_math.h"
#include <cmath>

namespace core_math
{
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

	


	std::vector<math_func::m_function> lib_core_math =
	{
		math_func::m_function("frac", static_cast<double(*)(double)>(frac)),
		math_func::m_function("factorial", static_cast<double(*)(double)>(factorial))
	};
};