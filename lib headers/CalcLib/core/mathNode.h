#ifndef MATHNODE_H_INCLUDED
#define MATHNODE_H_INCLUDED
#include "tree.h"
#include "modules/memory.h"
#include "modules/operators.h"
#include "main.h"
struct buildVector;
struct variableToken;
using tree::node;
using tree::nodeDataInterface;

namespace mathNode
{


typedef double number_type;

    struct nodeOops : public exception
    {
        nodeOops(std::string inf);
        const char* what();
    };

	class mathExpressionNode : public nodeDataInterface
	{
	public:
		mathExpressionNode();
		void bind(node * context);
		void destroy();
	};


   class mathExpressionNode_val : public mathExpressionNode
	{
	public:
		number_type value;
		mathExpressionNode_val();
		mathExpressionNode_val(number_type val);
		number_type eval();
	};

class mathExpressionNode_opr;

	class mathExpressionNode_variable : public mathExpressionNode
	{

	private:
		memory* mem;
		bool _stack;
		friend mathExpressionNode_opr;
	public:
		std::string name;
		mathExpressionNode_variable(std::string var, memory* mem, bool b);
		number_type eval();
		bool is_pushable();
		mathExpressionNode_variable(const mathExpressionNode_variable&);
		mathExpressionNode_variable operator=(const mathExpressionNode_variable&);

	};




	class mathExpressionNode_opr : public mathExpressionNode
	{
	public:

        union{ //Saves memory as these pointers will never be used at the same time

		operators::operPtr operation;
		operators::assigmentPtr assign;
        };
		bool assignB;


		mathExpressionNode_opr(operators::operPtr operation);
		mathExpressionNode_opr(operators::assigmentPtr assign);
        mathExpressionNode_opr(void);

		number_type eval();

	};



	class mathExpressionNode_func : public mathExpressionNode
	{
	public:
		number_type (*func)(number_type);
		typedef number_type (*funcPtr)(number_type);
		mathExpressionNode_func();
		mathExpressionNode_func(funcPtr operation);
		number_type eval();
	};

	class mathExpressionNode_func_tree : public mathExpressionNode
	{
	public:
		number_type(*func)(tree::node*);
		typedef number_type(*funcPtr)(tree::node*);
		mathExpressionNode_func_tree();
		mathExpressionNode_func_tree(funcPtr operation);
		number_type eval();
	};

	namespace helper
	{

		template<typename T> struct  enum_type
		{
			enum{ TYPE = -1 };
		};
		template<> struct enum_type < mathNode::mathExpressionNode_func* >
		{
			enum{ TYPE = tree::FUNCTION };
		};
		template<> struct enum_type < mathNode::mathExpressionNode_func_tree* >
		{
			enum{ TYPE = tree::FUNCTION_TREE };
		};
		template<> struct enum_type < mathNode::mathExpressionNode_val* >
		{
			enum{ TYPE = tree::VALUE };
		};
		template<> struct enum_type < mathNode::mathExpressionNode_opr* >
		{
			enum{ TYPE = tree::OPERATOR };
		};
		template<> struct enum_type < mathNode::mathExpressionNode_variable* >
		{
			enum{ TYPE = tree::VARIABLE };
		};



	}

}
#endif // MATHNODE_H_INCLUDED
