#ifndef MATHNODE_H_INCLUDED
#define MATHNODE_H_INCLUDED
#include "tree.h"
#include "memory.h"
template<typename mathType> struct buildVector;
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

	};




	struct mathExpressionNode_opr : public mathExpressionNode
	{


		typedef number_type (*operatorPtr)(number_type,number_type);
		typedef number_type (*assigmentPtr)(variableToken, number_type);
		operatorPtr operation;
		assigmentPtr assign;
		bool assignB;


		mathExpressionNode_opr(operatorPtr operation);
		mathExpressionNode_opr(assigmentPtr assign);
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
