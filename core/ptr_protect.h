#ifndef PTR_PROTECT_H
#define PTR_PROTECT_H
#include "exception_helper.h"
/**
 *This file contains templates for implementing a simple smart pointer interface
 */
 
/**
 * isPointer checks if the specifid type T is an pointer
 */
template<typename T> class isPointer
{
   public:
   /*
    * ptr is true if it is an pointer and flase if an value or reference
	*/
        enum{ptr=false}; 
};
template<typename T> class isPointer<T*>
{
      public:
    enum{ptr=true};
};
template<typename T> class isPointer<T**>
{
      public:
    enum{ptr=true};
};


template<typename T, bool D> 
class std_delete
{
    public:
    static void f(T x)
    {
        delete x;   
    }
};

template<typename T> 
class std_delete<T,true>
{
    public:
    static void f(T x)
    {
        delete[ ]x;;
    }
};

template<typename T, bool array_type, bool isPtr=isPointer<T>::ptr>
class ptr_protect
{

};

template<typename T, bool array_type>
class ptr_protect<T,array_type,false>
{

};


/**
 * A smart pointer object
 */

template<typename T,bool B>
class ptr_protect<T,B,true>
{
	typedef void(*del_func)(T);
    private:
        T _ptr;
        bool _enabled;
		del_func _f;
        void __d()
        {
             if(_enabled && _ptr != nullptr)
            {
				if (_f == nullptr)
				{
					std_delete<T, B>::f(_ptr);
					_ptr = nullptr;
				}
				else
				{
					_f(_ptr);
					_ptr = nullptr;
				}
            }
        }
    public:
		
        explicit ptr_protect(T ptr): _ptr(ptr), _enabled(true), _f(nullptr){}
		ptr_protect(T ptr,del_func f): _ptr(ptr), _enabled(true), _f(f){}
        ptr_protect(ptr_protect&& x) : _ptr(x._ptr), _enabled(true), _f(x._f)
	{
		if(!x._enabled)
		{
			x.release();	
			__mathlibra__raise<PTR_PROTECT_MOVED_FREED_PTR,PTR_PROTECT>();
		}
		x.release(); x._ptr=nullptr;
	}
        ptr_protect(): _ptr(nullptr), _enabled(true){}
        
        ~ptr_protect()
        {
            __d();    
        }
        void release()  {_enabled=false;} /**< Releases ownership. When called the smart pointer will no longer own the pointer and therefore not delete the object when the smart pointer goes out of scope.*/
        T ptr() const {return _ptr;} /**< @return An pointer to the object managed by the smart pointer */
        T operator->(){return _ptr;}
        ptr_protect& operator=(ptr_protect&& x)
        {
            __d();
            this->_ptr = x._ptr;
            this->_enabled = x._enabled;
			this->_f = x._f;
			x.release();
            x._ptr=nullptr;
            return *this;
        }
};

/**
 * allocates memory using a smart pointer. @return ptr_protect @see ptr_protect
 */
template<typename T> ptr_protect<T*,false,isPointer<T*>::ptr> safe_alloc()
{
    return ptr_protect<T*,false,isPointer<T*>::ptr>(new T);
}
/**
 * deallocates memory using a smart pointer. @return ptr_protect @see ptr_protect
 */
template<typename T> ptr_protect<T*,true,isPointer<T*>::ptr> safe_alloc(size_t n)
{
    return ptr_protect<T*,true,isPointer<T*>::ptr>(new T[n]);
}


#endif // PTR_PROTECT_H
