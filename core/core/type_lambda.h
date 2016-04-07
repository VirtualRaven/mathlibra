#include "type_interface.h"
#include "function_obj.h"
class lambda_type : public interface::type
{
	
public:
	size_t sizeN() 	const 		{return 1;}
	size_t sizeM() 	const 		{return 1;}
	bool isVector() const 		{return false;}
	bool isList() 	const		{return false;}
	bool isSingleton() const	{return false;}
	bool isString()	const 		{return false;}
	bool isNumber() const 		{return false;}
	bool isFunction() const		{return true;}
	storage_types stores() const	{return T_FUNC;}
};
