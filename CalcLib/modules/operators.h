#ifndef OPERATORS_H_INCLUDED
#define OPERATORS_H_INCLUDED

#include "main.h"
#include "modules/memory.h"
#include <cmath>
#include <vector>

struct variableToken; //Forward declared from core

namespace operators
{
    typedef number_type(*operPtr)(number_type,number_type);
    typedef number_type(*assigmentPtr)(memory* mem,std::string name, number_type);
    enum operationType{NOT_SET,ASSIGN,MATH};

    struct interpreter_operator
    {
		union
		{
			operPtr oper;
			assigmentPtr assign;
		};
        char operChar;
        short baseWheight;
        operationType operType;
        interpreter_operator(operPtr opr, char symbol, short wheight);
        interpreter_operator(assigmentPtr assign, char symbol, short wheight);
        interpreter_operator();
    };

   typedef std::vector<interpreter_operator> vec_operators;

    class operators_interface
    {
        vec_operators _operators;
        unsigned int lastOffset;
        public:
        void load(vec_operators oprs);
        void load(interpreter_operator tok);
        operators_interface();
        void getOperators();
        bool inArray(char opr);
        interpreter_operator getCurrent();
    };
    extern vec_operators std_operators;



}
#endif // OPERATORS_H_INCLUDED

