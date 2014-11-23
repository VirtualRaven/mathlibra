#include "tokens.h"
#include <cmath>
using mathNode::number_type;

namespace operators
{
    number_type add(number_type x, number_type y);
	number_type multi(number_type x, number_type y);
	number_type divide(number_type x, number_type y);
	number_type sub(number_type x, number_type y);
	number_type oper_pow(number_type x, number_type y);
    number_type oper_assign(variableToken storage, double val);


struct allOperators
{
	int lastOffset;
	operatorToken expressionArray[6];
	allOperators();
	bool inArray(char opr);
	operatorToken getCurrent();
};

}
