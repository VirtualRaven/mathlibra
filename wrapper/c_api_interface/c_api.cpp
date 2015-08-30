#include "c_api.h"
#include "native_class_wrapper.h"

struct handle_obj
{
 	native::core_native_wrapper wrp;
};

handle create_handle()
{
	return  new handle_obj;	
}
void free_handle(handle hwn)
{
	delete hwn;
}
void interpret_arg(handle hwn,char * str)
{
	hwn->wrp.set_arg(str);
	hwn->wrp.interpret_arg();		
}
double execute_arg(handle hwn)
{
	return hwn->wrp.execute_arg();	
}
bool mathlibra_error(handle hwn)
{
	return hwn->wrp.exceptionOccured();
}

const char* mathlibra_error_info(handle hwn)
{
	auto x = hwn->wrp.get_exception_info();
	std::string msg = x.type + std::string(":")+  x.desc;
	return msg.c_str();

}	

void enable_plugins(handle hwn)
{
	hwn->wrp.enablePlugins();

}
