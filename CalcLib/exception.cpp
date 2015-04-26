#include "exception.h"


exception::exception(std::string inf, bool isCritical) : info(inf),_isCritical(isCritical) {}

exception::exception():info(""),_isCritical(true){}
bool exception::critical()
{
	return _isCritical;
}
 exception::~exception()
 {

 }

 std::string exception::desc()
 {
	 return this->info;
 }