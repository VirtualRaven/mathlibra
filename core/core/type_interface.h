#include <string>
#include "ptr_protect.h"
namespace interface
{
    
    class type;

    enum storage_types
    {
        UNKN,
        DOUBLE,
        CHAR,
        TYPE
    };

    template<typename T> struct get_storage
    {
        static constexpr storage_types t() {return UNKN;} 
    };
    template<>  struct get_storage<double>
    {
        static constexpr storage_types t() { return DOUBLE;}
    };
    template<> struct get_storage<char>
    {
        static constexpr storage_types t() { return CHAR;}
    };
    template<>  struct get_storage<type>
    {
        static constexpr storage_types t() { return  TYPE;}
    };
    
    template<typename T> class iterator
    {

    };


    class type
    {
        public:
        virtual size_t  sizeN() const =0;
        virtual size_t  sizeM() const =0;
        /*
         *  Properties
         */
        virtual bool    isVector() const=0 ;
        virtual bool    isList() const =0;
        virtual bool    isSingleton() const=0 ;
        virtual bool    isString() const=0;
        virtual bool    isNumber() const =0;
        virtual storage_types stores() const=0 ;
        virtual ~type() = default;
        virtual type*   copy() const=0; /**> Deep copies the underlying type */
        //virtual type* reduce(); /**> Tries to convert the internal t_type to the least general one*/

        /*
         *  Convertors
         */
        virtual double  toNumber() const=0;   
        virtual const char*  toString()=0; /**< @returns a pointer valid until next operation on object */

    };
    typedef ptr_protect<type*,false> type_ptr;
    template<typename T> class   t_type : public type
    {
        public:
        /*
         *  Accessors
         */
        virtual T get(size_t n, size_t m)=0;
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
