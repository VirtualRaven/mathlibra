#include "c_api.h"
#include "native_class_wrapper.h"
#include <cstring>


struct handle_obj
{
 	native::core_native_wrapper wrp;
};


mem_obj to_c_struct(interface::mem_obj_api obj)
{
    mem_obj tmp = {new char[obj.name.size()],obj.value,obj.isConst};
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
    delete obj.name;   
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
double execute_arg(handle hwn)
{
	return hwn->wrp.execute_arg();	
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
void free_error_info(const char * info)
{
	delete info;
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
      
    return to_c_struct( hwn->wrp.getVariable((size_t)index));
}

mem_obj mem_get(handle hwn,char* name)
{
    return to_c_struct( hwn->wrp.getVariable(name));
}
void    mem_set(handle hwn,mem_obj obj)
{
    hwn->wrp.manageVariable(std::string(obj.name),obj.val,obj.const_specifier);       
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


void free_func_obj_array(func_obj_array obj);


