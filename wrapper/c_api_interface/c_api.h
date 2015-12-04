#include <stdbool.h>
#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct handle_obj*  handle;
	typedef unsigned int uint;
    extern handle create_handle();
	extern void free_handle(handle);
	extern void interpret_arg(handle,char *);
	extern double execute_arg(handle);
	extern void enable_plugins(handle);	

        //Error handling
    typedef struct
    {
        const char * type;
        const char * description;
        bool         critical;
        unsigned int id_number;

    } error_obj;

        //Returns the current error object and resets the error state.
    extern  error_obj mathlibra_error_info(handle);

        //Returns true if an error has occured. To clear the error call mathlibra_error_info
	extern bool mathlibra_error(handle);

        //Memory module
    typedef struct
    {
        char* name;
        double val;
        bool const_specifier;
    } mem_obj;

	typedef struct
	{
		const char* name;
		const char* tag;
		const char* doc;
		const char* disp_name;
	} func_obj;

	typedef struct
	{
		func_obj* array;
		unsigned int size;
	} func_obj_array;

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
