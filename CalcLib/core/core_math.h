#include "modules/functions.h"

// Vector containing mathematical functions implemeted in core
namespace core_math
{
	struct coreMathOops : public exception
	{
		coreMathOops(std::string inf, bool isCritical);
		const char* what();

	};
	extern std::vector<math_func::m_function> lib_core_math;
};