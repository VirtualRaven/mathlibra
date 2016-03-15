/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef TREE_INTERFACE_INCLUDED

#define TREE_INTERFACE_INCLUDED
#include <stack>
#ifdef TYPE_MEM_TEST
#include "object_counter.h"
#endif
#include "core/type_interface.h"
/**
 *@namespace tree Contains the tree classes. Provides the data types to orgenize data in a tree structure.
 */
namespace tree
{
	class node_base;
	/**
	 * The different types an token can have.
	 */
	enum tokenType
	{
		UNKNOWN = 0,  /**< Token type unset. If a node has this type it will be treated as an error*/
		VARIABLE = 1, /**< Variable type*/
		FUNCTION = 2, /**< Function type*/
		OPERATOR = 3, /**<Operator type*/
		VALUE = 4,	/**<Value type*/
		PARENTHESES = 5, /*< Paratheses type. Can not exist as an node.*/
		FUNCTION_TREE = 6, /**< A node that is a function that takes an variable number of arguments. @see math_func::m_function */
		DUMMY = 7, /**< A node that is a comma sign.  @note This type is only used to create multi argument functions, calling eval on a dummy node results in an exception.*/
                FUNCTION_USER =8
	};      

	/**
	 * A data interface for nodes. All nodes have to contain data which is of the type nodeDataInterface. Data like values, variables and function names are all stored using this interface.
	 */
	class nodeDataInterface
#ifdef TYPE_MEM_TEST
		: util::countable<nodeDataInterface>
#endif
	{
	protected:
		node_base * wrapperNode;
	public:
		friend  node_base * nodeDataInterface_wrapper_access(nodeDataInterface* node); /**<Provides extended access to the the class needed during construction of the tree.*/
		tokenType type; /**< The type of the node. @see tokenType. */

		virtual void bind(node_base * context)=0; /**< Called before operations on the node. The class has an internal pointer pointing back to the node_base which contains the data. This internal pointer needs to be set to the right node before any operations can be carried out. @note Forgetting to call bind before eval causes undefined behaviour. @param context Pointer to the node_base which owns the data. */
		virtual void destroy()=0; /**< Called upon tree destruction. It shall delete all data allocated by the class and then destroy the interface itself. @note Failure to correctly follow this behaviour causes memory leaks. */
		virtual bool isPure()=0; /**< Returns true if the call to eval has no side effects. If isPure is true it tells mathlibra that it is safe to cache the result of the calcualtion. */  
		virtual interface::type* eval()=0; /**< Evaluate node. When called is causes the node to evaluate itself. If the type of the data is one that takes input data the eval method is first called on the child branches of the node_base owning the data. Then this data is evaluated using the acquired data from the child branches. */

                /**
                 * Collapses the subtree as much as possible. 
                 * Provides higher executions speeds if called.
                 * @returns a pointer to the new optimized object, 
                 * the original object which the method
                 * was called upon may become invalid after the call
                 * thus use the returned object instead.
                 * @note Optimize updates the tree references so the node_base::data pointer
                 * is updated to point at the new object.
                 */
		virtual nodeDataInterface*  optimize()=0;
                virtual ~nodeDataInterface() = default;
	};

	/**
	 * The interface for the tree nodes. The abstract syntax tree is built up of several node_base to form a tree. Each node_base also contains an nodeDataInterface. @see nodeDataInterface.
	 */
	class node_base
	{
	public:

		nodeDataInterface* data; /**< The data belonging to the node.*/
	
		virtual node_base * sub1()=0; /**< Gets the first child branch. @return Pointer to the first child branch.*/
		virtual node_base * sub2()=0; /**< Gets the second child branch. @return Pointer to the second child branch.*/
		virtual std::stack<tree::node_base*> getArgs() = 0; /**<Used for multi argument functions. It searches the child branches an splits for DUMMY nodes. @see DUMMY. Using the dummy node it splits up the child-branches in several "sub trees" each coresonding to an argument for an function. @see FUNCTION_TREE. */
		virtual void raiseException(const char * inf)=0; /**<Raise an exception. This function is used to created an exception inside the core. @note It is primary thought to be used for passing exception over DLL-bundries, it is not made for internal use. \ For internal exceptions, overload the standard exception instead. @see exception. */
        virtual interface::type* realloc(interface::type* t)=0; /**< Reallocates the the type by deepcopying the object. Used to tranfer object allocation form shared library memory space to mathlibra's memoryspace */
		typedef void(*free_func)(interface::type*);
		virtual void free_type(interface::type*) = 0;
		virtual  free_func get_free_func()=0; /**< Get an pointer to an free function which deletes an type object. Used to free type object over dll bunderies" */
	};
}

#endif
