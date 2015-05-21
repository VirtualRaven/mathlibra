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

	number_type oper_assign(memory* mem,std::string name, double val)
	{
		mem->set(name,val);
		return val;
	}
	number_type dummy(number_type x, number_type y)
	{
		return 0;
	}

    vec_operators std_operators= {
        interpreter_operator(&sub,'-',1),
		interpreter_operator(&add,'+' ,1),
		interpreter_operator(&multi,'*' ,2),
		interpreter_operator(&divide,'/' ,2),
		interpreter_operator(&oper_pow,'^' ,3),
        interpreter_operator(&oper_assign,'=',0),
		interpreter_operator((operPtr)nullptr, ',', 0 )
    };

operators::interpreter_operator::interpreter_operator(operPtr opr, char symbol, short wheight):
  oper(opr),
  operChar(symbol),
  baseWheight(wheight),
  operType(MATH)
{}

operators::interpreter_operator::interpreter_operator(assigmentPtr assigne, char symbol, short wheight):
 assign(assigne),
 operChar(symbol),
 baseWheight(wheight),
 operType(ASSIGN)
{}

operators::interpreter_operator::interpreter_operator():
 oper(nullptr),
 operChar(0),
 baseWheight(0),
 operType(NOT_SET)
 {}




        void operators_interface::load(vec_operators oprs)
        {
            this->_operators.insert(_operators.end(),oprs.begin(),oprs.end());
        }
        void operators_interface::load(interpreter_operator tok)
        {
            this->_operators.push_back(tok);
        }

operators_interface::operators_interface()
: _operators(std_operators),
lastOffset(0)

{}

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
	interpreter_operator  operators_interface::getCurrent()
	{
		return this->_operators[lastOffset];
	}


}
