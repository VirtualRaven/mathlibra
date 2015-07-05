#ifndef MATHNODE_INTERFACE_H_INCLUDED
#define MATHNODE_INTERFACE_H_INCLUDED
#include "tree_interface.h"


using tree::node_base;
using tree::nodeDataInterface;

namespace memory
{
	class memory;
}

namespace operators
{
	typedef number_type(*operPtr)(number_type, number_type);
	typedef number_type(*assigmentPtr)(memory::memory* mem, std::string name, number_type);
}

namespace mathNode
{

	

	typedef double number_type;

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
		virtual bool is_pushable()=0; /**< @return True if variable can be pushed to the stack of the virtual machine. */

	};

	/**
	 * Interface for operator type nodes.
	 */
	class mathExpressionNode_opr_interface : public tree::nodeDataInterface
	{
	public:

		union{ //Saves memory as these pointers will never be used at the same time

			operators::operPtr operation;
			operators::assigmentPtr assign;
		};
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
			enum{ TYPE = -1 };
		};
		template<> struct enum_type < mathNode::mathExpressionNode_func_interface* >
		{
			enum{ TYPE = tree::FUNCTION };
		};
		template<> struct enum_type < mathNode::mathExpressionNode_func_tree_interface* >
		{
			enum{ TYPE = tree::FUNCTION_TREE };
		};
		template<> struct enum_type < mathNode::mathExpressionNode_val_interface* >
		{
			enum{ TYPE = tree::VALUE };
		};
		template<> struct enum_type < mathNode::mathExpressionNode_opr_interface* >
		{
			enum{ TYPE = tree::OPERATOR };
		};
		template<> struct enum_type < mathNode::mathExpressionNode_variable_interface* >
		{
			enum{ TYPE = tree::VARIABLE };
		};



	}

}
#endif