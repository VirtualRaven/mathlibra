/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef MATHNODE_INTERFACE_H_INCLUDED

#define MATHNODE_INTERFACE_H_INCLUDED
#include "tree_interface.h"
#include <string>
using interface::type;
using tree::node_base;
using tree::nodeDataInterface;

namespace memory
{
	class memory;
}

namespace operators
{
	typedef type* (*generic_oper_ptr) (tree::nodeDataInterface*);
}

namespace mathNode
{

	

		/**
	 * Interface for value type nodes.
	 */
	class mathExpressionNode_val_interface : public tree::nodeDataInterface
	{
            protected:
                interface::type_ptr value;
	    public:
		virtual interface::type* eval()=0;
	};
	/**
     *	Interface for variable type nodes.
	 */		 
	class mathExpressionNode_variable_interface :public tree::nodeDataInterface
	{

	public:
		std::string name;
		virtual void set(interface::type_ptr) = 0; /**<Sets the value of the variable. Creates an local copy of the data provided*/
		virtual interface::type* eval()=0;
                virtual bool is_undefined()=0;
		virtual bool is_pushable()=0; /**< @return True if variable can be pushed to the stack of the virtual machine. */

	};

	/**
	 * Interface for operator type nodes.
	 */
	class mathExpressionNode_opr_interface : public tree::nodeDataInterface
	{
	public:

		operators::generic_oper_ptr ptr;
		bool assignB; /**< True if the currently contained function in the node is of assigmentPtr type, else false. */
		
		virtual interface::type* eval()=0;

	};
	class mathExpressionNode_func_tree_interface : public tree::nodeDataInterface
	{
	public:
            interface::type*(*func)(tree::node_base*);
		typedef interface::type*(*funcPtr)(tree::node_base*);
		virtual interface::type* eval()=0;
	};
        
        class mathExpressionNode_func_user_interface : public tree::nodeDataInterface
        {
            virtual interface::type* eval()=0;
        };

        	
	/**
	 * @namespace helper contains functions to ease creation of templates.
	 */
	namespace helper
	{

		/**
		 *@struct enum_type  A struct used to map tokenType to an nodeDataInterface object.
		 *@tparam T the nodeDataInterface child class to get token type for.
		 */
		template<typename T> struct  enum_type
		{
			/**
			 * Contains the tokenType of T.
			 */
		 	static const tree::tokenType TYPE = tree::UNKNOWN;
		};
		template<> struct enum_type < mathNode::mathExpressionNode_func_tree_interface* >
		{
			static const tree::tokenType TYPE = tree::FUNCTION_TREE;
		};
		template<> struct enum_type < mathNode::mathExpressionNode_val_interface* >
		{
			static const tree::tokenType TYPE = tree::VALUE;
		};
		template<> struct enum_type < mathNode::mathExpressionNode_opr_interface* >
		{
			static const tree::tokenType TYPE = tree::OPERATOR;
		};
		template<> struct enum_type < mathNode::mathExpressionNode_variable_interface* >
		{
			static const tree::tokenType  TYPE = tree::VARIABLE;
		};



	}

}
#endif
