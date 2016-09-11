/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef RECURSION_TYPES_H
#define RECURSION_TYPES_H
#include <exception>
#include <initializer_list>
#include <string>
namespace recursion
{
        class index_out_of_range_exception : public std::exception
        {
            public:
            index_out_of_range_exception() {}
            virtual const char* what() const noexcept
            {
                return "fixed_array: index out of range";
            }
        };

        template<int N,typename T> class array_slice;
        template<int N,typename T> class fixed_array
        {
            T a[N];   
            public:
            T& operator[](int i)
            {
                if( i>=0 && i < N)
                    return a[i]; 
                else throw index_out_of_range_exception();
            }
            fixed_array() {};
            fixed_array(std::initializer_list<T> l)
            {
                if(l.size() != N)
                    throw index_out_of_range_exception();
                int i2=0;
                for (auto i = l.begin(); i != l.end(); i2++,i++)
                    a[i2]=*i;
            }
            array_slice<N,T> tail()
            {
                return array_slice<N,T>(a); 
            }   
        };

        template<int N,typename T> class array_slice
        {   
            protected:
                const T* const array;
                array_slice(const T* a1) : array(a1) {}
                friend array_slice<N,T> fixed_array<N,T>::tail();
                friend array_slice<N-1,T>::array_slice(const array_slice<N,T>&);
            public:

            array_slice(const array_slice<N+1,T>& slice ) : array(&slice.array[1]) {}
            array_slice<N-1,T> tail()
            {
                return array_slice<N-1,T>(*this);
            }
            const T& head()
            {
                return *array;
            }
        };

        template<typename T> class array_slice<1,T>
        {   
            private:
                const T* const array;
            protected:
                array_slice(const T* a1) : array(a1) {}
                friend array_slice<1,T> fixed_array<1,T>::tail();
            public:

            array_slice(const array_slice<2,T>& slice ) : array(&slice.array[1]) {}
            const T& head()
            {
                return *array;
            }
        };

};
#endif 
