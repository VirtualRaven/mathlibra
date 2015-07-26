#ifndef PTR_PROTECT_H
#define PTR_PROTECT_H
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

template<typename T>
class ptr_protect<T,true,true>
{
    public:
        ptr_protect(T ptr): _ptr(ptr), _enabled(true){};
        virtual ~ptr_protect()
        {
            if(_enabled && _ptr != nullptr)
            {
                delete[] _ptr;
                _ptr=nullptr;
            }
        };
        void release()  {_enabled=false;}; /**< Releases ownership. When called the smart pointer will no longer own the pointer and therefore not delete the object when the smart pointer goes out of scope.*/
        T ptr(){return _ptr;}; /**< @return An pointer to the object managed by the smart pointer */
    private:
        T _ptr;
        bool _enabled;
};

template<typename T>
class ptr_protect<T,false,true>
{
    public:
        ptr_protect(T ptr): _ptr(ptr), _enabled(true){};
        virtual ~ptr_protect()
        {
         if(_enabled && _ptr != nullptr)
            {
                delete _ptr;
                _ptr=nullptr;
            }
        };
        void release()  {_enabled=false;};  /**< Releases ownership. When called the smart pointer will no longer own the pointer and therefore not delete the object when the smart pointer goes out of scope.*/
        T ptr(){return _ptr;}; /**< @return An pointer to the object managed by the smart pointer */
		T operator ->(){ return _ptr; }; /**< @return An pointer to the object managed by the smart pointer */
    private:
        T _ptr;
        bool _enabled;


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
