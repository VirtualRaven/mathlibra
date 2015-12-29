#include "exception_helper.h"
#include "type_interface.h"
#include <iterator>
#include <string>
using interface::type; 



template<typename T,typename F> std::string __toString(T* obj, size_t  n, size_t  m,F f)
{
    std::string tmp;
    tmp.reserve(4*n*m);
    tmp.push_back('[');
    for(size_t  i = 0; i < n; i++)
    {   
        if( m > 1 &&  n != 1)
        {
            tmp.push_back('[');
        }
        for(size_t  j =0; j < m; j++)
        {
            tmp += f(obj[i*n+j]);
            if(j != m-1)
            {
                tmp.push_back(',');
            }
        }
        if(m > 1&& n != 1) 
        {
            tmp.push_back(']');
        }
        if(i != n -1)
        {
            tmp.push_back(',');
        }  
    }
    tmp.push_back(']');
    return tmp;
}

template<typename T> std::string _t_toString(T*, size_t , size_t  ){ return ""; }
template<>  std::string _t_toString<char>(char* d, size_t  n, size_t  m)
{
    return __toString<char>(d,n,m,[](char x){return x;});
}
template<> std::string _t_toString<double>(double* d, size_t  n, size_t  m)
{
    return __toString<double>( d, n, m, [](double x){return std::to_string(x);});
}
template<> std::string _t_toString<interface::type*>(interface::type** d, size_t  n, size_t  m)
{
    return __toString<interface::type*>(d,n,m,[](interface::type* t){return t->toString();});
}



template<typename T,bool C=false> class type_iterator : std::iterator<std::random_access_iterator_tag,T>
{   
    int _m;
    int _n;
    int _i;

    T* _mat;
    int _to_index(const int& x)
    {
        if(C)
        {
            return  (x-x%_n)/_n + (x%_n)*_m; 
        }
        else
        {
            return x;
        }
    }

    public: 
    friend int operator-(const type_iterator<T>& x,const type_iterator<T>& y)
    {
        return x._i - y._i;   
    }
    friend type_iterator<T,C> operator+(type_iterator<T,C> x, const int& n)
    {
        x._i+=n;
    }
    friend type_iterator<T,C> operator+(const int& n, type_iterator<T,C> x)
    {
        return x+n;
    }
    friend type_iterator<T,C> operator-(type_iterator<T,C> x, const int& n)
    {
        return x+(-n);
    }
    friend bool operator<(const type_iterator<T,C>& x, const type_iterator<T,C>& y)
    {
        return x._i < y._i;
    } 
    friend bool operator >(const type_iterator<T,C>& x, const type_iterator<T,C>& y)
    {
        return x._i > y._i;
    }
    friend bool operator>=(const type_iterator<T,C>& x, const type_iterator<T,C>& y)
    {
        return x._i >=  y._i;
    }
    
    friend bool operator<=(const type_iterator<T,C>& x, const type_iterator<T,C>& y)
    {
        return x._i <= y._i;
    }
    T& operator*()
    {
        return (*this)[_i];
    }
    T& operator[](int n)
    {
       return _mat[_to_index(n)];
    } 
    type_iterator(int m, int n, int i, T* mat): _m(m),_n(n),_i(i),_mat(mat) {}
    
    type_iterator& operator+=(int n)
    {
       _i += n;
       return *this;
    }
    type_iterator& operator-=(int n)
    {
       return this->operator+=(-n);
    }
    type_iterator& operator++()
    {
        return this->operator+=(1);
    }
    type_iterator operator++(int)
    {
         auto tmp =*this;
         this->operator++();
         return tmp; 
    }
    bool   operator==(const type_iterator& x)
    {
       return this->_m ==x._m && this->_n == x._n && this->_i == x._i && this->_mat == x._mat;
    }
    bool operator!=(const type_iterator& x)
    {
        return !this->operator==(x);
    }
    

};

template<typename T> class   base_type : public interface::t_type<T>
{   
    size_t  _m;
    size_t  _n;
    T* _mat;
    std::string _str;
    public:

    size_t  sizeN() const
    {
        return _n;
    };
    size_t  sizeM() const
    {
        return _m;
    }
    /*
     *  Iterators
     */

    type_iterator<T> begin()
    {
        return type_iterator<T>(_m,_n,0,_mat);
    }
    type_iterator<T> end()
    {
        return type_iterator<T>(_m,_n,_m*_n,_mat);
    }
    type_iterator<T> back()
    {
        return --this->end();
    }
    type_iterator<T,true> begin_c()
    {
        return type_iterator<T,true>(_m,_n,0,_mat);
    }
    type_iterator<T,true> end_c()
    {
        return type_iterator<T,true>(_m,_n,_m*_n,_mat);
    }
    type_iterator<T,true> back_c()
    {
        return --this->end_c();
    }
    /*
     *  Properties
     */
        bool    isVector() const
        {
            return (_n > 1) && (_m == 1 );
        }
        bool    isList() const
        {
            return (_n == 1) && (_m > 1);
        }
        bool    isSingleton() const 
        {
            return (_n == 1 && _m == 1);
        }
        bool    isString() const 
        {
            return isList() && this->stores() == interface::storage_types::CHAR;
        }
        bool    isNumber() const
        {
            return isSingleton() && this->stores() == interface::storage_types::DOUBLE;
        }
        T   toSingleton() const 
        {
            if(isSingleton())
            {
                return _mat[0];
            }
        }

        double  toNumber() const
        {
            if(this->isNumber() ) 
            {
                return static_cast<double>(toSingleton());
            }
        }
        
        const char*   toString()
        {
            _str = _t_toString<T>(_mat,_n,_m);
            return _str.c_str();
        }

        T   get(size_t  n, size_t m )
        {
            if(n < this->sizeN() && m < this->sizeM())
            {
                return _mat[_n*n+m];
            } 
            else
            {

            }  
        }
        base_type(T* d,size_t n, size_t m) : 
        _m(m),
        _n(n),    
        _mat(new T[n*m]),
        _str()
        {
                memcpy(_mat, d,m*n*sizeof(T)); 
        }
        base_type(const base_type& b) : base_type(b._mat,b._n,b._m) {};   
        base_type(base_type&& b):
            base_type(b._mat,b.sizeN(),b.sizeM())
        {
            b._mat=nullptr;
            b._n=0;
            b._m=0;   
        }
        interface::type* copy() const
        {
            return new base_type(*this);
        }
         
        ~base_type()
        {   
            delete[] _mat;
        }
};  
typedef base_type<char> char_mat;
typedef base_type<type*> mat_mat;
typedef base_type<double> num_mat;
