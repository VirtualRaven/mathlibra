#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED
#include <string>
#include "main.h"


class exception
{
protected:
	std::string info;

public:

	exception(std::string inf);
	exception();
	virtual ~exception();
	virtual const char * what();
    std::string desc();


};
#endif //EXCEPTION_H_INCLUDED
