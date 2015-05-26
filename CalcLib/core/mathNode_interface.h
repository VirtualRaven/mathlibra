#ifndef MATHNODE_INTERFACE_H_INCLUDED
#define MATHNODE_INTERFACE_H_INCLUDED
#include "tree_interface.h"
#include "modules/operators.h"

using tree::node_base;
using tree::nodeDataInterface;

namespace mathNode
{


	typedef double number_type;

	class mathExpressionNode_val_interface : public tree::nodeDataInterface
	{
	public:
		number_type value;
		virtual number_type eval()=0;
	};

	class mathExpressionNode_variable_interface :public tree::nodeDataInterface
	{

	public:
		std::string name;
		virtual number_type eval()=0;
		virtual bool is_pushable()=0;

	};


	class mathExpressionNode_opr_interface : public tree::nodeDataInterface
	{
	public:

		union{ //Saves memory as these pointers will never be used at the same time

			operators::operPtr operation;
			operators::assigmentPtr assign;
		};
		bool assignB;
		virtual number_type eval()=0;

	};

	class mathExpressionNode_func_interface : public tree::nodeDataInterface
	{
	public:
		number_type(*func)(number_type);
		typedef number_type(*funcPtr)(number_type);
		virtual number_type eval()=0;
	};

	class mathExpressionNode_func_tree_interface : public tree::nodeDataInterface
	{
	public:
		number_type(*func)(tree::node_base*);
		typedef number_type(*funcPtr)(tree::node_base*);
		virtual number_type eval()=0;
	};

	namespace helper
	{

		template<typename T> struct  enum_type
		{
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