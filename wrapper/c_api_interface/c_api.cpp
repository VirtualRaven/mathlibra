#include "c_api.h"
#include "native_class_wrapper.h"
#include <cstring>

struct return_obj
{
	interface::type_ptr x;
};

struct handle_obj
{
 	native::core_native_wrapper wrp;
	return_obj __return_val;
};

const char* toStr(ret t)
{
	return t->x->toString();
}
bool   isNumber(ret t)
{
	return t->x->isNumber();
}
double toNumber(ret t)
{
	return t->x->toNumber();
}

mem_obj to_c_struct(interface::mem_obj_api obj)
{
    ret val = new return_obj {interface::type_ptr(obj.value)} ;
    val->x.release(); //We don't own the pointer in obj so we have to make sure type_ptr does not free it. 
    mem_obj tmp = {new char[obj.name.size()],val,obj.isConst};
    strcpy(tmp.name, obj.name.c_str()); 
    return tmp;

}

const char* to_c_str(std::string x)
{
  char * str = new char[x.size()];
  strcpy(str,x.c_str());
  return str;
}

void free_mem_obj(mem_obj obj)
{
    delete[] obj.name;  
    delete  obj.value;
}

handle create_handle()
{
	auto tmp=  new handle_obj;
	return tmp;
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
ret execute_arg(handle hwn)
{
	hwn->__return_val.x =  hwn->wrp.execute_arg();
	return &hwn->__return_val;
}
bool mathlibra_error(handle hwn)
{
	return hwn->wrp.exceptionOccured();
}

error_obj  mathlibra_error_info(handle hwn)
{
	auto x = hwn->wrp.get_exception_info();
        //Initlize an new error_obj struct and return it
        return {x.type, x.desc,x.isCritical, x.id };        
}	

void enable_plugins(handle hwn)
{
	hwn->wrp.enablePlugins();

}

uint  mem_size(handle hwn)
{
    return  hwn->wrp.getNumVariables();    
}
mem_obj mem_get_index(handle hwn,uint index)
{
      auto tmp=hwn->wrp.getVariable((size_t)index);
      if(!hwn->wrp.exceptionOccured())
      {
    	return to_c_struct(tmp);
      }
      else
	{
		return {};
	}
}

mem_obj mem_get(handle hwn,char* name)
{
	auto tmp=hwn->wrp.getVariable(name);
	if(!hwn->wrp.exceptionOccured())
	{
    		return to_c_struct(tmp);
	}
	else
	{
		return {};
	}
}
void    mem_set(handle hwn,mem_obj obj)
{
    hwn->wrp.manageVariable(std::string(obj.name),interface::type_ptr(obj.value->x->copy()),obj.const_specifier);       
}

func_obj_array func_get(handle hwn)
{
	auto vec = hwn->wrp.getFunctionObjs();
	unsigned int size = vec.size();
	func_obj* ptr = new func_obj[size];
	for(unsigned int i =0; i < size; i++)
	{
		ptr[i].name =to_c_str(vec[i].name);
		ptr[i].doc =to_c_str(vec[i].doc);
		ptr[i].tag =to_c_str(vec[i].tag);
		ptr[i].disp_name =to_c_str(vec[i].disp_name);
	}
	return {ptr,size};
}
inline void free_func_obj(func_obj obj)
{
	delete obj.name;
	delete obj.tag;
	delete obj.doc;
	delete obj.disp_name;	
}
void free_func_obj_array(func_obj_array obj)
{
	
	for(unsigned int i=0; i < obj.size; i++)
	{
		free_func_obj(obj.array[i]);
	}
	delete obj.array;
}




