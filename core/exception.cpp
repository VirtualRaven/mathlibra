#include "exception.h"
 char __exception_buffer[128] = {'\0'};

exception::exception(const char* type,
	  const char* inf,
	  bool isCritical ,
	  unsigned short id 
	  	) : _info(inf),
		     _type(type),
		    _id(id),
		    _isCritical(isCritical) {}

bool exception::critical()
{
	return _isCritical;
}

unsigned short exception::id()
{
	return this->_id;
}

const char * exception::what()
{
	return this->_type;
}


 const char *  exception::desc()
 {
	 return this->_info;
 }
