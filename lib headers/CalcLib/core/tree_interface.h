#ifndef TREE_INTERFACE_INCLUDED
#define TREE_INTERFACE_INCLUDED
#include <stack>
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
		UNKNOWN = 0,  /**< Token type unset. If an node has this type it will be treated as an error*/
		VARIABLE = 1, /**< Variable type*/
		FUNCTION = 2, /**< Function type*/
		OPERATOR = 3, /**<Operator type*/
		VALUE = 4,	/**<Value type*/
		PARENTHESES = 5, /*< Paratheses type. Can not exist as an node.*/
		FUNCTION_TREE = 6, /**< An node that is an function that takes an variable number of arguments. @see math_func::m_function */
		DUMMY = 7 /**< An node that is an comma sign.  @note This type is only used to create multi argument functions, calling eval on a dummy node results in an exception.*/
	};

	class nodeDataInterface
	{
	protected:
		node_base * wrapperNode;
	public:
		friend  node_base * nodeDataInterface_wrapper_access(nodeDataInterface* node);
		tokenType type;
		//Is called every time the a root node want to make an operation on an tree
		//By standard bind couses the node to synch their pointer to root the root with the pointer passed.

		virtual void bind(node_base * context)=0;
		virtual void destroy()=0;
		//Evaluate node, calculates an value based upon node children
		virtual double eval()=0;

	};

	
	/**
	 * The interface for the tree nodes. The abstract syntax tree is built up of several node_base to form a tree. Each node_base also contains an nodeDataInterface. @see nodeDataInterface. 
	 */

	class node_base
	{
	public:

		nodeDataInterface* data;
		//First branch of tree
		virtual node_base * sub1()=0;
		//Second bracnh of tree
		virtual node_base * sub2()=0;
		//Crates an stack of the arguments provided to the function
		virtual std::stack<tree::node_base*> getArgs() = 0;
		//Raises an exception in the interpreter
		virtual void raiseException(const char * inf)=0;
		
	};
}

#endif