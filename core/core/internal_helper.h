/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "core/tree.h"
#include "core/mathNode_interface.h"
#include "modules/function_helper.h"
#include "core/type_helper.h"
namespace internal_helper
{
        /*
         * forward_fast is an faster impementation. It is faster as it runs under the assumption that the function only takes one or two arguments depending on the overload, and thus do not have to travers the tree. This function is intended for implementing unary and binary operators inside mathlibra.
         *
         * 
         *
         * WARNING
         *  These forward_fast implementations does not handle automatic exception wrapping and memory transfer over dll bundires that the normal forward functions do.
         *  Using these functions from an plugin will result in undefined behaviour and/or memory leaks.
         */ 
	template<typename T, typename T2> type* forward_fast(typename function_helper::func_type<T, T2>::f_type func, tree::nodeDataInterface*n)
	{
		auto wrapper = tree::nodeDataInterface_wrapper_access(n);
		try
		{
			return func(function_helper::getData<T>(wrapper->sub1()), function_helper::getData<T2>(wrapper->sub2()));
		}
		catch (std::exception& e)
		{
			wrapper->raiseException(e.what());
			return nullptr;
		}	
	}
        
	template<typename T, typename T2> type* forward_fast(typename function_helper::func_type_double<T, T2>::f_type func, tree::nodeDataInterface*n)
	{
		auto wrapper = tree::nodeDataInterface_wrapper_access(n);
		try
		{
			return make_type(func(function_helper::getData<T>(wrapper->sub1()), function_helper::getData<T2>(wrapper->sub2())));
		}
		catch (std::exception& e)
		{
			wrapper->raiseException(e.what());
			return nullptr;
		}	
	}

        template<typename T> type* forward_fast(typename  function_helper::func_type<T>::f_type func, tree::node_base* n)
        {
			try
			{
				return func(function_helper::getData<T>(n->sub1()));
			}
			catch (std::exception& e)
			{
				n->raiseException(e.what());
				return nullptr;
			}
        }

        template<typename T> type* forward_fast(typename  function_helper::func_type_double<T>::f_type func, tree::node_base* n)
        {
			try
			{
				return make_type(func(function_helper::getData<T>(n->sub1())));
			}
			catch (std::exception& e)
			{
				n->raiseException(e.what());
				return nullptr;
			}
        }
    
}
