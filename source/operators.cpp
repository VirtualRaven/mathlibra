#include "operators.h"


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




	allOperators::allOperators()
	{
		expressionArray[0]= operatorToken(&sub,'-',1);
		expressionArray[1]=	operatorToken(&add,'+' ,1);
		expressionArray[2]=	operatorToken(&multi,'*' ,2);
		expressionArray[3]=	operatorToken(&divide,'/' ,2);
		expressionArray[4]=	operatorToken(&oper_pow,'^' ,3);
		expressionArray[5]= operatorToken(&oper_assign,'=',0);
		lastOffset=0;
	}


	bool allOperators::inArray(char opr)
	{
		for(int i =0; i< 6; i++)
		{
			if(expressionArray[i].operChar == opr)
			{
				lastOffset =i;
				return true;
			}
		}
		return false;
	}
	operatorToken allOperators::getCurrent()
	{
		return expressionArray[lastOffset];
	}


}
