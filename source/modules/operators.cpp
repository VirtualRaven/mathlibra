#include "modules/operators.h"


namespace operators
{

	number_type add(number_type x, number_type y)
	{
		return x+y;
	}

    number_type multi(number_type x, number_type y)
	{
		return x*y;
	}

	number_type divide(number_type x, number_type y)
	{
		return x/y;
	}

	number_type sub(number_type x, number_type y)
	{
		return x-y;
	}

    number_type oper_pow(number_type x, number_type y)
	{
		return pow(x,y);
	}

	number_type oper_assign(variableToken storage, double val)
	{
		storage.mem->set(storage.variableName,val);
		return val;
	}

    operators std_operators= {
        operatorToken(&sub,'-',1),
		operatorToken(&add,'+' ,1),
		operatorToken(&multi,'*' ,2),
		operatorToken(&divide,'/' ,2),
		operatorToken(&oper_pow,'^' ,3),
        operatorToken(&oper_assign,'=',0),
    };


        void operators_interface::load(operators oprs)
        {
            this->_operators.insert(_operators.end(),oprs.begin(),oprs.end());
        }
        void operators_interface::load(operatorToken tok)
        {
            this->_operators.push_back(tok);
        }

operators_interface::operators_interface()
{
    this->lastOffset=0;
    this->_operators = std_operators;
}

	bool  operators_interface::inArray(char opr)
	{
		for(unsigned int i =0; i< this->_operators.size(); i++)
		{
			if(this->_operators[i].operChar == opr)
			{
				lastOffset =i;
				return true;
			}
		}
		return false;
	}
	operatorToken  operators_interface::getCurrent()
	{
		return this->_operators[lastOffset];
	}


}
