/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED
#include "type_interface.h"
#include "ptr_protect.h"
#include <iterator>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring> //memcpy
using interface::type; 



namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

template<typename T,typename F> std::string __toString(T* obj, size_t  n, size_t  m,F f)
{
    std::string tmp;
    tmp.reserve(4*n*m);
	if (n > 1 && m != 1)
	{
		tmp.push_back('[');
	}
	for(size_t  i = 0; i < n; i++)
    	{   
       		if( m > 1)
       		{
       		    tmp.push_back('[');
       		}
       		for(size_t  j =0; j < m; j++)
       		{
       		    tmp += f(obj[i*m+j]);
       		    if(j != m-1)
       		    {
       		        tmp.push_back(',');
       		    }
       		}
       		if(m > 1) 
       		{
       		    tmp.push_back(']');
       		}
       		if(i != n -1)
       		{
       		    tmp.push_back('|');
       		}  
    	}
	if (n > 1 && m != 1)
	{
		tmp.push_back(']');
		tmp.push_back('\n');
	}
    return tmp;
}

template<typename T> std::string _t_toString(T*, size_t , size_t  ){ return ""; }


template<>  inline std::string _t_toString<char>(char* d, size_t  n, size_t  m)
{
	if(n==1)
	{
		return "\"" + std::string(d,0,m) + "\""; 
	}
    	return __toString<char>(d,n,m,[](char x){return x;});
}

template<> inline std::string _t_toString<double>(double* d, size_t  n, size_t  m)
{
    return __toString<double>( d, n, m, [](double x){return patch::to_string(x);});
}
template<> inline std::string _t_toString<interface::type*>(interface::type** d, size_t  n, size_t  m)
{
    return __toString<interface::type*>(d,n,m,[](interface::type* t){return t->toString();});
}


/**
 *Iterator for base type.
 *The iterator iterates though all elements inside base_type either by column or row.
 * @warning This iterator does not preform any bounds checking!
 *@tparam T the type of the value inside the base_type
 *@tparam C true if the iterator should iterator by column instead of row.
 */
template<typename T,bool C=false> class type_iterator : public std::iterator<std::random_access_iterator_tag,T>
{  public: 
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
    friend int operator-(const type_iterator& x,const type_iterator& y)
    {
        return x._i - y._i;   
    }
    friend type_iterator operator+(const type_iterator& x, const int& n)
    {
        return type_iterator(x)+=n ;
    }
    friend type_iterator operator+(const int& n, type_iterator x)
    {
        return x+n;
    }
    friend type_iterator operator-(const type_iterator& x, const int& n)
    {
        return x+(-n);
    }
    friend bool operator<(const type_iterator& x, const type_iterator& y)
    {
        return x._i < y._i;
    } 
    friend bool operator >(const type_iterator& x, const type_iterator& y)
    {
        return x._i > y._i;
    }
    friend bool operator>=(const type_iterator& x, const type_iterator& y)
    {
        return x._i >=  y._i;
    }
    
    friend bool operator<=(const type_iterator& x, const type_iterator& y)
    {
        return x._i <= y._i;
    }
    T& operator*()
    {
        return (*this)[_i];
    }
    T* operator->()
    {
    	return _mat+_to_index(_i);
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
    type_iterator& operator--()
    {
        return this->operator-=(1);   
    }
    type_iterator  operator--(int)
    {
        auto tmp = *this;
        this->operator--();
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
template<typename D> double __toNumber(D*)
{
         return 0;
}
template<> inline double __toNumber<double>(double* x)
{
        return x[0];
}
/**
 *The templated implementation of mathlibras data types.
 *@see type
 */
template<typename T> class   base_type : public interface::t_type<T>
{   
    size_t  _m;
    size_t  _n;
    T* _mat;
    std::string _str;
   
    public:
		T* raw()
		{
			return _mat;
		}
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
    template<bool by_column=false> 
    type_iterator<T,by_column> begin() /**< Returns an iterator to the begin. @tparm by_column selects the type of iterator. */
    {
        return type_iterator<T,by_column>(_m,_n,0,_mat);
    }
    template<bool by_column=false> 
    type_iterator<T,by_column> end()
    {
        return type_iterator<T,by_column>(_m,_n,_m*_n,_mat);
    }
    template<bool by_column=false> 
    type_iterator<T,by_column> back()
    {
        return --this->end<T,by_column>();
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
            return isList() && this->stores() == storage_types::T_CHAR;
        }
        bool    isNumber() const
        {
            return isSingleton() && this->stores() == storage_types::T_DOUBLE;
        }
        T   toSingleton() const 
        {
                return _mat[0];
        }

        double  toNumber() const
        {
            return __toNumber<T>(this->_mat);
        }
        
        const char*   toString()
        {
            _str = _t_toString<T>(_mat,_n,_m);
            return _str.c_str();
        }

        T   get(size_t  n, size_t m )
        {
                return _mat[(_n-1)*n+m];
        }
	/**
	 *base_type constructor
	 *@param d  An array of lenght n*m containing data that will be copied  into the base_type-
	 *@param n  The number of columns that the base_type should have.
	 *@param m  The number of rows that the base_type should have.
	 **/
        base_type(const T* d,size_t n, size_t m) : 
        _m(m),
        _n(n),    
        _mat(new T[n*m]),
        _str()
        {
                memcpy(_mat, d,m*n*sizeof(T)); 
        }
	/**
	 *@param n The number of columns that the base_type should have.
	 *@param m  The number of rows that the base_type should have.
	 **/
	base_type(size_t n, size_t m) :
	_m(m),
	_n(n),
	_mat(new T[n*m]),
	_str()
	{}

	/**
	 *@param d A smart pointer containing the data array to be moved into base_type. 
	 *@note base_type takes ownership of the smart pointer and its data.
	 *@note Use this constuctor to create base types without any unessecary copying.
	 *@param n the number of columns that the base_type should have.
	 *@param m the number of rows that the base_type should have.
	 **/
	template<bool B> base_type(ptr_protect<T*,B>&& d,size_t n,size_t m): 
		base_type(d.ptr(),n,m)
	{
		d.release();
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
/**
 * Template functions to get an base_type's coresponding  storage_types
 */
template<typename T> struct get_enum
{
    static constexpr storage_types t() {return T_UNKN;} 
};
template<>  struct get_enum<num_mat>
{
    static constexpr storage_types t() { return T_DOUBLE;}
};
template<> struct get_enum<char_mat>
{
    static constexpr storage_types t() { return T_CHAR;}
};
template<>  struct get_enum<mat_mat>
{
    static constexpr storage_types t() { return  T_TYPE;}
};
#endif //TYPE_H_INCLUDED
