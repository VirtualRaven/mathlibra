#include <stdbool.h>
#include "core/type_enum.h"
#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct handle_obj*  handle;
	typedef  void*  ret;
	typedef unsigned int uint;
    extern handle create_handle();
	extern void free_handle(handle);
	extern void interpret_arg(handle,char *);
	extern ret execute_arg(handle); /**< Executes the currently interpreted expression.@returns an return_obj pointer. The pointer is tied to the handle and is invalidated when the handle is freed or the a new call is made to execute_arg with the same handle. @note There is no need to free the return value from this function */
	extern void enable_plugins(handle);	

	/*
	 * Return object functions
	 */


	extern const char* toStr(ret); /**< @returns a string representing the object, this string is an valid mathlibra expression. 
										@note Pointer only valid until the next call of an mathlibra function. */
	extern bool   isNumber(ret);  /**< Checks if ret's data_type is double an the internal list only contains one element */
	extern double toNumber(ret); /**< @returns the return object converted to an double if ret is an number, else zero is returned */
	extern unsigned int sizeM(ret); /**< @returns The number of culumns in ret */
	extern unsigned int sizeN(ret); /**< @returns The number of rows in ret */
	extern storage_types getStorage(ret); /**< @returns The underlying storage type */
	extern double* getDoubleArray(ret); /**< Creates an pointer to a 1d double array. Creats an pointer to the underlying data, 
										in this case an double array. This presumes that the data_type of ret is double. 
										@returns An pointer to the ret's internal double array which is valid through the lifetime
										of the object. For matrices the data is stored by rows. 
										@example If ret contains a matrix consisting of 
										the column vectors {1,1,1} {2,2,2} and {3,3,3} the returned pointer
										represents the following list { 1,2,3,1,2,3,1,2,3}
										matrix getDoubleArray would return  
										@note Using this function on an ret that does not have an data_type of double
										causes undefined behavioure */

	extern char*   getCharArray(ret); /**< Same as getDoubleArray(ret) but for char types.  
									  @see  getDoubleArray(ret) */
	extern ret    getTypeArray(ret); /**< Same as getDoubleArray(ret) but for recursive ret types.
									  @see  getDoubleArray(ret) */
	/*
	 * Error handling functions.
	 */
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
	
	/*
	 * Function functions :)
	 */
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


	/*
	 * Memory functions
	 */

        //Memory module
   	 typedef struct
   	 {
   	     char* name;
   	     ret value; /**> Pointer to the variables value. @note value points to the internal variables value, so by modifying value you directly modifies the variables value */
	     bool const_specifier;
   	 } mem_obj;

	   	 extern uint  mem_size(handle); /**>@returns Integer representing the number of variables in memory     */
   	 extern mem_obj mem_get_index(handle,uint index); //| O(n)
   	 extern mem_obj mem_get(handle,char* name);  //| O(1)
   	 extern void    mem_set(handle,mem_obj obj); /**< Creates or updates an variable using obj. @note If you only wish to update an variables value there is no need to call this function, instead use mem_get or mem_get_index to get an mem_obj an then modify it's value instead */
   	 extern void free_mem_obj(mem_obj obj); /**> Free mem_obj memory.Must be called on the return values of mem_get functions after the calle is done with the object to free the memory.*/

#ifdef __cplusplus
}
#endif
