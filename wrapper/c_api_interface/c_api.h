#ifdef __cplusplus
extern "C"
{
#endif
	struct handle_obj;
	typedef handle_obj*  handle;
	typedef unsigned int uint; 
        
        extern handle create_handle();
	extern void free_handle(handle);
	extern void interpret_arg(handle,char *);
	extern double execute_arg(handle);
	extern bool mathlibra_error(handle);
	extern const char* mathlibra_error_info(handle);
	extern void enable_plugins(handle);	

        //Memory module
       struct mem_obj
       {
              char* name;
              double val;
              bool const_specifier;
       };

	struct func_obj
	{
		const char* name;
		const char* tag;
		const char* doc;
		const char* disp_name;
	};
	
	struct func_obj_array
	{
		func_obj* array;
		unsigned int size;
	};
       	
	extern func_obj_array func_get(handle);	
	extern void free_func_obj_array(func_obj_array obj);

        extern uint  mem_size(handle); /**>@returns Integer representing the number of variables in memory     */ 
        extern mem_obj mem_get_index(handle,uint index); //| O(n)
        extern mem_obj mem_get(handle,char* name);  //| O(1)
        extern void    mem_set(handle,mem_obj obj); //| O(1)
        extern void free_mem_obj(mem_obj obj); /**> Free mem_obj memory.Must be called on the return values of mem_get functions after the calle is done with the object to free the memory.*/

#ifdef __cplusplus
}
#endif
