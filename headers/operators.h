#include "tokens.h"
#include <cmath>


namespace operators
{
	template<typename mathType>
	mathType add(mathType x, mathType y)
	{
		return x+y;
	}

	template<typename mathType>
	mathType multi(mathType x, mathType y)
	{
		return x*y;
	}

	template<typename mathType>
	mathType divide(mathType x, mathType y)
	{
		return x/y;
	}

	template<typename mathType>
	mathType sub(mathType x, mathType y)
	{
		return x-y;
	}
	template<typename mathType>
	mathType oper_pow(mathType x, mathType y)
	{
		return pow(x,y);
	}

	template<typename mathType>
	mathType oper_assign(variableToken<mathType> storage, double val)
	{
		storage.mem->set(storage.variableName,val);
		return val;
	}



template<typename mathType> struct allOperators
{
	int lastOffset;
	operatorToken<mathType> expressionArray[6];

	allOperators()
	{
		expressionArray[0]= operatorToken<mathType>(&sub,'-',1);
		expressionArray[1]=	operatorToken<mathType>(&add,'+' ,1);
		expressionArray[2]=	operatorToken<mathType>(&multi,'*' ,2);
		expressionArray[3]=	operatorToken<mathType>(&divide,'/' ,2);
		expressionArray[4]=	operatorToken<mathType>(&oper_pow,'^' ,3);
		expressionArray[5]= operatorToken<mathType>(&oper_assign,'=',0);
		lastOffset=0;
	}


	bool inArray(char opr)
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
	operatorToken<mathType> getCurrent()
	{
		return expressionArray[lastOffset];
	}

};

}
