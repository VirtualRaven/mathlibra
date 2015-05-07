#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED
#include <string>
#include "main.h"
class exception
{
protected:
	std::string info;
	bool _isCritical;
public:

	exception(std::string inf,bool isCritical);
	exception();
	virtual ~exception();
	virtual const char * what()=0;
	bool critical();
    std::string desc();


};
#endif //EXCEPTION_H_INCLUDED
