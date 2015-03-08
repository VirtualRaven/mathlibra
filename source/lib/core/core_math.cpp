#include "core_math.h"
#include <cmath>

namespace core_math
{

	unsigned long  integerReqiured(double arg) // Throws exception if not convertable to integer
	{
		if ((fabs(round(arg) - arg) <= 0.00001f)) //Check if value is an integer within given bounds
		{
			return (unsigned long)arg;
		}
		else
		{
			throw math_func::functionOops("frac: invalid argument");
		}
	}

	double frac(double arg) 
	{
		unsigned long argI = integerReqiured(arg);
			
			std::cout << "{ ";
			for (unsigned int i = 2; i <= floor(sqrt(argI)); i++)
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
		unsigned long argI = integerReqiured(arg);
		for (unsigned long i = argI - 1; i > 1; i--)
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