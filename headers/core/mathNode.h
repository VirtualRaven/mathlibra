#ifndef MATHNODE_H_INCLUDED
#define MATHNODE_H_INCLUDED
#include "tree.h"
#include "modules/memory.h"
#include "modules/operators.h"
#include "main.h"
struct buildVector;
struct variableToken;
namespace mathNode
{


typedef double number_type;

    struct nodeOops : public exception
    {
        nodeOops(std::string inf);
        const char* what();
    };

	struct mathExpressionNode : public nodeDataInterface
	{
		mathExpressionNode();
		void bind(node * context);
		void destroy();
	};


    struct mathExpressionNode_val : public mathExpressionNode
	{
		number_type value;
		mathExpressionNode_val();
		mathExpressionNode_val(number_type val);
		number_type eval();
	};

struct mathExpressionNode_opr;

	struct mathExpressionNode_variable : public mathExpressionNode
	{

	private:
		memory* mem;
		friend mathExpressionNode_opr;
	public:
		std::string name;
		mathExpressionNode_variable(std::string var, memory* mem);
		number_type eval();
		mathExpressionNode_variable(const mathExpressionNode_variable&);
		mathExpressionNode_variable operator=(const mathExpressionNode_variable&);

	};




	struct mathExpressionNode_opr : public mathExpressionNode
	{


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



	struct mathExpressionNode_func : public mathExpressionNode
	{
		number_type (*func)(number_type);
		typedef number_type (*funcPtr)(number_type);
		mathExpressionNode_func();
		mathExpressionNode_func(funcPtr operation);
		number_type eval();
	};

}
#endif // MATHNODE_H_INCLUDED
