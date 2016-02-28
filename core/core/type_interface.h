#ifndef TYPE_INTERFACE_H_INCLUDED
#define TYPE_INTERFACE_H_INCLUDED
#include <string>
#include "ptr_protect.h"
#include "type_enum.h"
#ifdef TYPE_MEM_TEST
#include "object_counter.h"
#endif
namespace interface
{
    
    class type;



    template<typename T> struct get_storage
    {
        static constexpr storage_types t() {return T_UNKN;} 
    };
    template<>  struct get_storage<double>
    {
        static constexpr storage_types t() { return T_DOUBLE;}
    };
    template<> struct get_storage<char>
    {
        static constexpr storage_types t() { return T_CHAR;}
    };
    template<>  struct get_storage<type*>
    {
        static constexpr storage_types t() { return  T_TYPE;}
    };
    
/** 
 * Interface for type.
 * Type is the base class for mathlibra's data types.
 * @note  When mathlibra is built in debug mode this 
 * class inherets from util::countable<type> to make 
 * object allocation statistics and memory leak detection possible.
 * @see util::countable
 *
 */ 
    class type
#ifdef TYPE_MEM_TEST
	:public util::countable<type>
#endif
    {
        public:
        virtual size_t  sizeN() const =0;/**< Returns the number of rows in the underlying object. */
        virtual size_t  sizeM() const =0;/**< Returns the number of columns in the underlying object */
        /*
         *  Properties
         */
        virtual bool    isVector() const=0 ; 
        virtual bool    isList() const =0;
        virtual bool    isSingleton() const=0 ; /**<Checks if the size of the type is 1x1*/
        virtual bool    isString() const=0; /**< Checks if the type is an string. */
        virtual bool    isNumber() const =0; /**< Checks if the data is an number. */
        virtual storage_types stores() const=0 ; /**< Returns an enum representing which child class the interface contains. @returns storage_types. @example if 
						   the returned value is T_DOUBLE this implies that the type pointer can be safely cast into num_mat. */
        virtual ~type() = default; /**< Default constructor */
        virtual type*   copy() const=0; /**< Deepcopies the object. 
					  	@returns type* pointing to an copy of the type. 
						@note this copy is allocated on the heap an thus
       						it is the users responsibility to delete it when done*/

        /*
         *  Convertors
         */
        virtual double  toNumber() const=0;   /**< Converts the type into an double if possible. @note This method only returns an valid value if isNumber() returns true, else toNumber() returns 0.*/
        virtual const char*  toString()=0; /**< @returns a pointer valid until next operation on object */

    };
    typedef ptr_protect<type*,false> type_ptr;
    template<typename T> class   t_type : public type
    {
        public:
        /*
         *  Accessors
         */
        virtual T get(size_t n, size_t m)=0; /**< Gets the value stored in row n column m*/
        storage_types stores() const
        {
            return get_storage<T>::t();
        }
        type* type()
        {
            return static_cast<interface::type*>(this);
        }
        virtual ~t_type() = default;
    };


   
    }
#endif //TYPE_INTERFACE_H_INCLUDED
