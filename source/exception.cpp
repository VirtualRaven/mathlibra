#include "exception.h"


exception::exception(std::string inf)
	{
		info = inf;
	}
exception::exception()
	{
		info = "";
	}

	const char * exception::what()
	{
		return "";
	}
	 std::string exception::desc()
	{
		 return info;
	}
