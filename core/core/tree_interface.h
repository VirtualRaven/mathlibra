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
		UNKNOWN = 0,  /**< Token type unset. If a node has this type it will be treated as an error*/
		VARIABLE = 1, /**< Variable type*/
		FUNCTION = 2, /**< Function type*/
		OPERATOR = 3, /**<Operator type*/
		VALUE = 4,	/**<Value type*/
		PARENTHESES = 5, /*< Paratheses type. Can not exist as an node.*/
		FUNCTION_TREE = 6, /**< A node that is a function that takes an variable number of arguments. @see math_func::m_function */
		DUMMY = 7 /**< A node that is a comma sign.  @note This type is only used to create multi argument functions, calling eval on a dummy node results in an exception.*/
	};

	/**
	 * A data interface for nodes. All nodes have to contain data which is of the type nodeDataInterface. Data like values, variables and function names are all stored using this interface.
	 */
	class nodeDataInterface
	{
	protected:
		node_base * wrapperNode;
	public:
		friend  node_base * nodeDataInterface_wrapper_access(nodeDataInterface* node); /**<Provides extended access to the the class needed during construction of the tree.*/
		tokenType type; /**< The type of the node. @see tokenType. */

		virtual void bind(node_base * context)=0; /**< Called before operations on the node. The class has an internal pointer pointing back to the node_base which contains the data. This internal pointer needs to be set to the right node before any operations can be carried out. @note Forgetting to call bind before eval causes undefined behaviour. @param context Pointer to the node_base which owns the data. */
		virtual void destroy()=0; /**< Called upon tree destruction. It shall delete all data allocated by the class and then destroy the interface itself. @note Failure to correctly follow this behaviour causes memory leaks. */
		virtual double eval()=0; /**< Evaluate node. When called is causes the node to evaluate itself. If the type of the data is one that takes input data the eval method is first called on the child branches of the node_base owning the data. Then this data is evaluated using the acquired data from the child branches. */

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

	};
}

#endif
