#ifndef OPERATORS_H_INCLUDED
#define OPERATORS_H_INCLUDED

#include "main.h"
#include "modules/memory.h"
#include "core/tree_interface.h"
#include <cmath>
#include <vector>

struct variableToken; //Forward declared from core
/**
 * @namespace operators Contains the operator module.
*/
namespace operators
{
	typedef number_type(*generic_oper_ptr)(tree::nodeDataInterface*);


    struct interpreter_operator
    {
		generic_oper_ptr ptr;
        char operChar;
        short baseWheight;
        interpreter_operator(generic_oper_ptr opr, char symbol, short wheight);
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

