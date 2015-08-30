#include "c_api.h"
#include "native_class_wrapper.h"

handle create_handle()
{
	return static_cast<void*>( new native::core_native_wrapper);	
}
void free_handle(handle hwn)
{
	native::core_native_wrapper* wrp = static_cast<native::core_native_wrapper*>(hwn);
	delete wrp;
}
void interpret_arg(handle hwn,char * str)
{
	native::core_native_wrapper* wrp = static_cast<native::core_native_wrapper*>(hwn);
	wrp->set_arg(str);
	wrp->interpret_arg();		
}
double execute_arg(handle hwn)
{
	native::core_native_wrapper* wrp = static_cast<native::core_native_wrapper*>(hwn);
	return wrp->execute_arg();	
}
bool mathlibra_error(handle hwn)
{
	native::core_native_wrapper* wrp = static_cast<native::core_native_wrapper*>(hwn);
	return wrp->exceptionOccured();
}
void enable_plugins(handle hwn)
{
	native::core_native_wrapper* wrp = static_cast<native::core_native_wrapper*>(hwn);
	wrp->enablePlugins();

}
