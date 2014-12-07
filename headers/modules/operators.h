#ifndef OPERATORS_H_INCLUDED
#define OPERATORS_H_INCLUDED

#include "core/tokens.h"
#include "main.h"
#include <cmath>
#include <vector>

using mathNode::number_type;

namespace operators
{
   typedef std::vector<operatorToken> operators;

    class operators_interface
    {
        operators _operators;
        unsigned int lastOffset;
        public:
        void load(operators oprs);
        void load(operatorToken tok);
        operators_interface();
        void getOperators();
        bool inArray(char opr);
        operatorToken getCurrent();
    };
    extern operators std_operators;



}
#endif // OPERATORS_H_INCLUDED

