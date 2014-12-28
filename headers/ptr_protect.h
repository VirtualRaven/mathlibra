#ifndef PTR_PROTECT_H
#define PTR_PROTECT_H

template<typename T> class isPointer
{
   public:
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
        void release()  {_enabled=false;};
        T ptr(){return _ptr;};
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
        void release()  {_enabled=false;};
        T ptr(){return _ptr;};

    private:
        T _ptr;
        bool _enabled;
};


template<typename T> ptr_protect<T*,false,isPointer<T*>::ptr> safe_alloc()
{
    return ptr_protect<T*,false,isPointer<T*>::ptr>(new T);
}
template<typename T> ptr_protect<T*,true,isPointer<T*>::ptr> safe_alloc(size_t n)
{
    return ptr_protect<T*,true,isPointer<T*>::ptr>(new T[n]);
}


#endif // PTR_PROTECT_H
