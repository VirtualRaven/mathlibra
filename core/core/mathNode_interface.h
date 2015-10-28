#ifndef MATHNODE_INTERFACE_H_INCLUDED
#define MATHNODE_INTERFACE_H_INCLUDED
#include "tree_interface.h"
#include <string>

using tree::node_base;
using tree::nodeDataInterface;
namespace mathNode
{
  typedef double number_type;
}
namespace memory
{
	class memory;
}

namespace operators
{
	typedef mathNode::number_type (*generic_oper_ptr) (tree::nodeDataInterface*);
}

namespace mathNode
{

	

		/**
	 * Interface for value type nodes.
	 */
	class mathExpressionNode_val_interface : public tree::nodeDataInterface
	{
	public:
		number_type value;
		virtual number_type eval()=0;
	};
	/**
     *	Interface for variable type nodes.
	 */		 
	class mathExpressionNode_variable_interface :public tree::nodeDataInterface
	{

	public:
		std::string name;
		virtual void set(number_type) = 0; /**<Sets the value of the variable*/
		virtual number_type eval()=0;
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
		
		virtual number_type eval()=0;

	};
	/**
	 * Interface for function type nodes. 
	 *  Only functions accepting an single argument are allowed in this node.
	 */
	class mathExpressionNode_func_interface : public tree::nodeDataInterface
	{
	public:
		number_type(*func)(number_type);
		typedef number_type(*funcPtr)(number_type);
		virtual number_type eval()=0;
	};
	/**
	 * Interface for function_tree type nodes.
	 * Functions accepting an variable number of arguments are allowed in this node 
	 */
	class mathExpressionNode_func_tree_interface : public tree::nodeDataInterface
	{
	public:
		number_type(*func)(tree::node_base*);
		typedef number_type(*funcPtr)(tree::node_base*);
		virtual number_type eval()=0;
	};
        
        class mathExpressionNode_func_user_interface : public tree::nodeDataInterface
        {
            virtual number_type eval()=0;
        };

        	
	/**
	 * @namesoace helper contains functions to ease creation of templates.
	 */
	namespace helper
	{

		/**
		 *@struct enum_type an structure used to map tokenType to an nodeDataInterface object.
		 *@tparam T the nodeDataInterface child class to get token type for.
		 */
		template<typename T> struct  enum_type
		{
			/**
			 * Contains the tokenType of T.
			 */
		 	static const tree::tokenType TYPE = tree::UNKNOWN;
		};
		template<> struct enum_type < mathNode::mathExpressionNode_func_interface* >
		{
			static const tree::tokenType TYPE = tree::FUNCTION;
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
