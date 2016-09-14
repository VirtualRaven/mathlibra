/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "c_api.h"
#include "native_class_wrapper.h"
#include <cstring>

using  interface::type;


struct handle_obj
{
 	native::core_native_wrapper wrp;
	interface::type_ptr __return_val;
};

const char* toStr(ret t)
{
	return static_cast<type*>(t)->toString();
}
bool   isNumber(ret t)
{
	return static_cast<type*>(t)->isNumber();
}
double toNumber(ret t)
{
	return static_cast<type*>(t)->toNumber();
}

extern unsigned int sizeM(ret t)
{
	return static_cast<type*>(t)->sizeM();
}
extern unsigned int sizeN(ret t)
{
	return static_cast<type*>(t)->sizeN();
}
extern storage_types getStorage(ret t)
{
	return static_cast<type*>(t)->stores();
}

extern double* getDoubleArray(ret t)
{	
	auto tm = static_cast<type*>(t);
#ifndef UNSAFE_C_API
if (tm->stores() == storage_types::T_DOUBLE)
{
#endif
	return static_cast<num_mat*>(tm)->raw();
#ifndef UNSAFE_C_API
}
else
{
	return nullptr;
}
#endif
}

extern char* getCharArray(ret t)
{
	auto tm = static_cast<type*>(t);
#ifndef UNSAFE_C_API
	if (tm->stores() == storage_types::T_CHAR)
	{
#endif
		return static_cast<char_mat*>(tm)->raw();
#ifndef UNSAFE_C_API
	}
	else
	{
		return nullptr;
	}
#endif
}

extern ret getTypeArray(ret t)
{
	auto tm = static_cast<type*>(t);
#ifndef UNSAFE_C_API
	if (tm->stores() == storage_types::T_TYPE)
	{
#endif
		return static_cast<mat_mat*>(tm)->raw();
#ifndef UNSAFE_C_API
	}
	else
	{
		return nullptr;
	}
#endif
}



mem_obj to_c_struct(interface::mem_obj_api obj)
{
    mem_obj tmp = {new char[obj.name.size()],obj.value,obj.isConst};
    strncpy(tmp.name, obj.name.c_str(), obj.name.size());
    return tmp;

}

const char* to_c_str(std::string x)
{
  char * str = new char[x.size()];
  strncpy(str,x.c_str(), x.size());
  return str;
}

void free_mem_obj(mem_obj obj)
{
    delete[] obj.name;
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
	hwn->__return_val =  hwn->wrp.execute_arg();
	return hwn->__return_val.ptr();
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
    hwn->wrp.manageVariable(std::string(obj.name),
						interface::type_ptr(static_cast<type*>(obj.value)->copy()),
						obj.const_specifier);       
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
                ptr[i].signature =to_c_str(vec[i].signature);
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




