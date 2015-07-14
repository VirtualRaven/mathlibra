#include "modules/operators.h"
#include "core/internal_helper.h"

namespace operators
{

	number_type add(number_type x, number_type y)
	{
		return x+y;
	}
	number_type __add(tree::nodeDataInterface*n)
	{
		return internal_helper::forward_fast<double,double>(add, n);
	}

    number_type multi(number_type x, number_type y)
	{
		return x*y;
	}
	number_type __multi(tree::nodeDataInterface* n)
	{
		return internal_helper::forward_fast<number_type, number_type>(multi, n);
	}
	number_type divide(number_type x, number_type y)
	{
		return x/y;
	}
	number_type __divide(tree::nodeDataInterface* n)
	{
		return internal_helper::forward_fast<number_type, number_type>(divide, n);
	}
	number_type sub(number_type x, number_type y)
	{
		return x-y;
	}
	number_type __sub(tree::nodeDataInterface* n)
	{
		return internal_helper::forward_fast<number_type, number_type>(sub, n);
	}
	number_type __pow(tree::nodeDataInterface* n)
	{
		return internal_helper::forward_fast<number_type,number_type>( pow,n);
	}

	number_type assign(mathNode::mathExpressionNode_variable_interface* var, number_type val)
	{
		var->set(val);
		return val;
	}

	number_type __assign(tree::nodeDataInterface* n)
	{
		return internal_helper::forward_fast<mathNode::mathExpressionNode_variable_interface*, number_type>(assign, n);
	}
	
	
	number_type dummy(tree::nodeDataInterface*n)
	{
		return 0;
	}

    vec_operators std_operators= {
        interpreter_operator(&__sub,'-',1),
		interpreter_operator(&__add,'+' ,1),
		interpreter_operator(&__multi,'*' ,2),
		interpreter_operator(&__divide,'/' ,2),
		interpreter_operator(&__pow,'^' ,3),
        interpreter_operator(&__assign,'=',0),
		interpreter_operator((generic_oper_ptr)nullptr, ',', 0 )
    };

operators::interpreter_operator::interpreter_operator(generic_oper_ptr opr, char symbol, short wheight):
  ptr(opr),
  operChar(symbol),
  baseWheight(wheight)
{}

operators::interpreter_operator::interpreter_operator():
 ptr(nullptr),
 operChar(0),
 baseWheight(0)
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
