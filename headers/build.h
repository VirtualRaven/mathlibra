
#include <iostream>
#include "operators.h"
//Contains function for creating an exression tree

template<typename mathType>
struct buildVector
{
	short lowLimit;
	short hiLimit;
	unsigned short vecOffset;
	std::vector<baseToken*>* vecPtr;
	buildVector(short lowLimit, short hiLimit, short vecOffset, std::vector<baseToken*>* vecPtr)
	{
		this->lowLimit = lowLimit;
		this->hiLimit = hiLimit;
		this->vecOffset = vecOffset;
		this->vecPtr = vecPtr;
	}
	buildVector()
	{
		this->lowLimit = 0;
		this->hiLimit = 0;
		this->vecOffset = 0;
		this->vecPtr = nullptr;
	}
	//Return types, -1 error, >0 next operator index
	short calculateNextOperation(short lowLimit, short hiLimit)
	{
		short lowWheight = 9999;
		short nextOperation = -2;
		short lenght = 0;
		short values = 0;
		std::vector<baseToken*> vecLocal = *vecPtr;
		for (int i = lowLimit; i < hiLimit; i++)
		{

			if (vecLocal[i]->type == OPERATOR && static_cast<operatorToken*>(vecLocal[i])->baseWheight <= lowWheight)  //Check for next least important operator
			{
				lowWheight = static_cast<operatorToken*>(vecLocal[i])->baseWheight;
				nextOperation = i;
				lenght++;
			}
			else if (vecLocal[i]->type != PARENTHESES && vecLocal[i]->type != UNKNOWN)
			{
				if (vecLocal[i]->type == VALUE || vecLocal[i]->type == VARIABLE)
				{
					values++;
				}
				lenght++;
			}
			//Debug function remove later
			else if (vecLocal[i]->type == UNKNOWN)
			{
				std::cerr << "-[ Found unknown token ]\n";
			}
		}

		if (lowWheight != 9999) //A operator was found
		{
			return nextOperation;
		}
		else if (values == 1 && lenght == 1)	//There is nothing more to do, a value is the only member of the interval
		{
			for (int i = lowLimit; i < hiLimit; i++)
			{
				if (vecLocal[i]->type == VALUE || vecLocal[i]->type == VARIABLE)
				{
					return i;
				}
			}
		}
		//Implement function checking

		//Find error if program comes here
		std::cerr << "-[ Unknown error ]\n";
		return -2;
	}

};

template<typename mathType>
bool buildSubNodes(mathNode::mathExpressionNode * target, buildVector<mathType> vec)
{

	switch (target->type)
	{
	case VARIABLE:
		return true;
			break;
	case OPERATOR:
		return _operator_build(static_cast<mathNode::mathExpressionNode_opr*>(target), vec);
			break;
	case VALUE:
		return true;

	default:
	{
			   std::cerr << "Can't build subnodes, unsopported type\n";
		return false;
	    break;
	}
	}

}


template<typename mathType>
bool _operator_build(mathNode::mathExpressionNode_opr * tgt, buildVector<mathType> vec)
		{

			std::vector<baseToken*> vecLocal = *vec.vecPtr;
			buildVector<mathType> node1(0,0,0,vec.vecPtr);
			buildVector<mathType> node2(0,0,0,vec.vecPtr);

			//Simple syntax checking for the operators. Implement more advanced checking
			if(vec.vecOffset <= 0 )
			{
				std::cerr << "Syntax error: Expected value or exression before operator\n";
				return false;

			}
			else if(vecLocal[vec.vecOffset-1]->type == tokenType::OPERATOR) //Won't work if user writes any formating token before the operator
			{
				std::cerr << "Syntax error: Unexpected operator before operator\n";
				return false;
			}
			else if( vec.vecOffset >= vecLocal.size()-1 )
			{
				std::cerr << "Syntax error: Expected value or exression after operator\n";

			}
			else if(vecLocal[vec.vecOffset+1]->type == tokenType::OPERATOR) //Won't work if user writes any formating token before the operator
			{
				std::cerr << "Syntax error: Unexpected operator after operator\n";
				return false;
			}


			node1.lowLimit=vec.lowLimit;
			node1.hiLimit=vec.vecOffset;

			//Create first sub branche

			short result = node1.calculateNextOperation(node1.lowLimit, node1.hiLimit);
			if(result < 0)
			{
				return false;
			}

			mathNode::mathExpressionNode* mathNode1;
			if(vecLocal[result]->hasNode())
			{
				mathNode1 = vecLocal[result]->node();
			}
			else
			{
				std::cerr << "Can't create sub node!\n";
				return false;
			}

			node1.vecOffset=result;

			node2.lowLimit = vec.vecOffset + 1;
			node2.hiLimit = vec.hiLimit;
			result =	node2.calculateNextOperation(node2.lowLimit,node2.hiLimit);
			if(result < 0)
			{
				return false;
			}

			mathNode::mathExpressionNode* mathNode2;

			if(vecLocal[result]->hasNode())
			{
				mathNode2 = vecLocal[result]->node();
			}
			else
			{
				std::cerr << "Can't create sub node!\n";
				return false;
			}
			node2.vecOffset=result;





			nodeDataInterface_wrapper_access(tgt)->createSubNodes(mathNode1, mathNode2);
			if( !buildSubNodes(mathNode1,node1) || 	!buildSubNodes(mathNode2,node2))
			{
				return false;
			}

			return true;
		}

