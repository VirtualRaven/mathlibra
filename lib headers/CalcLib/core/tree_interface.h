#ifndef TREE_INTERFACE_INCLUDED
#define TREE_INTERFACE_INCLUDED
#include <stack>
namespace tree
{
	class node_base;
	enum tokenType
	{
		UNKNOWN = 0,
		VARIABLE = 1,
		FUNCTION = 2,
		OPERATOR = 3,
		VALUE = 4,
		PARENTHESES = 5,
		FUNCTION_TREE = 6,
		DUMMY = 7
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