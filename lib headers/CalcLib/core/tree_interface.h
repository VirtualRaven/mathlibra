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
		//Warning this behaviour must be implemented by the children of nodeDataInterface
		virtual void bind(node_base * context);
		virtual void destroy();
		nodeDataInterface();
		virtual ~nodeDataInterface();
		virtual double eval()
		{
			return 0;
		}

	};

	class node_base
	{
	public:

		nodeDataInterface* data;

		virtual node_base * sub1()=0;
		virtual node_base * sub2()=0;
		virtual std::stack<tree::node_base*> getArgs() = 0;
		virtual void raiseException(const char * inf)=0;
		virtual ~node_base()=0;
	};
}

#endif