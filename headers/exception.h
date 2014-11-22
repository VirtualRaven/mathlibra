#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED
#include <string>



class exception
{
protected:
	std::string info;

public:
	
	exception(std::string inf)
	{
		info = inf;
	}
	exception()
	{
		info = "";
	}
	
	virtual const char * what()
	{
		return "";
	}
	 std::string desc()
	{
		 return info;
	}

};
#endif //EXCEPTION_H_INCLUDED