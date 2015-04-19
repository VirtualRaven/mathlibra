#include "build.h"

using tree::rootNode;

	buildVector::buildVector(short lowLimit, short hiLimit, short vecOffset, std::vector<baseToken*>* vecPtr)
	:lowLimit(lowLimit),
	hiLimit(hiLimit),
	vecOffset(vecOffset),
	vecPtr(vecPtr)
	{}
	buildVector::buildVector()
	:lowLimit(0),
	hiLimit(0),
	vecOffset(0),
	vecPtr(nullptr)
	{}
	//Return types, -1 error, >0 next operator index
	short buildVector::calculateNextOperation(short lowLimit, short hiLimit)
	{
		short lowWheight = 9999;
		short nextOperation = -2;
		short lenght = 0;
		short values = 0;
		baseToken* basePtr = nullptr;
		for (int i = lowLimit; i < hiLimit; i++)
		{
			 
			basePtr = vecPtr->operator[](i);
			if (basePtr->type == tree::OPERATOR && static_cast<operatorToken*>(basePtr)->baseWheight <= lowWheight)  //Check for next least important operator
			{
				lowWheight = static_cast<operatorToken*>(basePtr)->baseWheight;
				nextOperation = i;
				lenght++;
			}
			else if (basePtr->type == tree::FUNCTION && static_cast<funcToken*>(basePtr)->baseWheight <= lowWheight)  //Check for next least important operator
			{
				lowWheight = static_cast<funcToken*>(basePtr)->baseWheight;
				nextOperation = i;
				lenght++;
			}
			else if (basePtr->type != tree::PARENTHESES &&basePtr->type != tree::UNKNOWN)
			{
				if (basePtr->type == tree::VALUE || basePtr->type == tree::VARIABLE)
				{
					values++;
				}
				lenght++;
			}

			//Debug function remove later
			else if (basePtr->type == tree::UNKNOWN)
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
				basePtr = vecPtr->operator[](i);
				if (basePtr->type == tree::VALUE || basePtr->type == tree::VARIABLE)
				{
					return i;
				}
			}
		}


		//Find error if program comes here
		std::cerr << "-[ Unknown error ]\n";
		return -2;
	}

bool _operator_build(mathNode::mathExpressionNode_opr * tgt, buildVector vec)
		{

			
			buildVector node1(0,0,0,vec.vecPtr);
			buildVector node2(0,0,0,vec.vecPtr);

			//Simple syntax checking for the operators. Implement more advanced checking
			if(vec.vecOffset <= 0 )
			{
				std::cerr << "Syntax error: Expected value or exression before operator\n";
				return false;

			}
			else if (vec.vecPtr->operator[](vec.vecOffset - 1)->type == tokenType::OPERATOR) //Won't work if user writes any formating token before the operator
			{
				std::cerr << "Syntax error: Unexpected operator before operator\n";
				return false;
			}
			else if( vec.vecOffset >= vec.vecPtr->size()-1 )
			{
				std::cerr << "Syntax error: Expected value or exression after operator\n";

			}
			else if(vec.vecPtr->operator[](vec.vecOffset+1)->type == tokenType::OPERATOR) //Won't work if user writes any formating token before the operator
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
			if(vec.vecPtr->operator[](result)->hasNode())
			{
				mathNode1 = vec.vecPtr->operator[](result)->node();
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

			if (vec.vecPtr->operator[](result)->hasNode())
			{
				mathNode2 = vec.vecPtr->operator[](result)->node();
			}
			else
			{
				std::cerr << "Can't create sub node!\n";
				return false;
			}
			node2.vecOffset=result;




			if (tgt->operation == nullptr && tgt->operation == nullptr)
			{
				tgt->type = tree::DUMMY;
			}
			nodeDataInterface_wrapper_access(tgt)->createSubNodes(mathNode1, mathNode2);
			if( !buildSubNodes(mathNode1,node1) || 	!buildSubNodes(mathNode2,node2))
			{
				return false;
			}

			return true;
		}

bool _function_build(mathNode::mathExpressionNode_func * tgt, buildVector vec)
{
	if (vec.vecOffset >= vec.vecPtr->size() - 1)
	{
		std::cerr << "Syntax error: Expected value or exression after function\n";

	}
	else if (vec.vecPtr->operator[](vec.vecOffset + 1)->type != tokenType::PARENTHESES) //Won't work if user writes any formating token before the operator
	{
		std::cerr << "Excpected parentheses after function name";
		return false;
	}
	for (int i = vec.lowLimit; i < vec.vecOffset; i++)
	{
		if (vec.vecPtr->operator[](i)->type != tree::PARENTHESES)
		{
			std::cerr << "Vital token on lefhand side of function in build vector!\n";
			return false;
		}
	}
	buildVector node1(vec.vecOffset + 1, vec.hiLimit, 0, vec.vecPtr);
	//Create first sub branche
	short result = node1.calculateNextOperation(node1.lowLimit, node1.hiLimit);
	if (result < 0)
	{
		return false;
	}
	mathNode::mathExpressionNode* mathNode1;
	if (vec.vecPtr->operator[](result)->hasNode())
	{
		mathNode1 = vec.vecPtr->operator[](result)->node();
	}
	else
	{
		std::cerr << "Can't create sub node!\n";
		return false;
	}

	node1.vecOffset = result;
	nodeDataInterface_wrapper_access(tgt)->createSubNodes(mathNode1,nullptr);
	if (!buildSubNodes(mathNode1, node1))
	{
		return false;
	}

	return true;
}

bool _function_build_tree(mathNode::mathExpressionNode_func_tree * tgt, buildVector vec)
{
	if (vec.vecOffset >= vec.vecPtr->size() - 1)
	{
		std::cerr << "Syntax error: Expected value or exression after function\n";

	}
	else if (vec.vecPtr->operator[](vec.vecOffset + 1)->type != tokenType::PARENTHESES) //Won't work if user writes any formating token before the operator
	{
		std::cerr << "Excpected parentheses after function name";
		return false;
	}
	for (int i = vec.lowLimit; i < vec.vecOffset; i++)
	{
		if (vec.vecPtr->operator[](i)->type != tree::PARENTHESES)
		{
			std::cerr << "Vital token on lefhand side of function in build vector!\n";
			return false;
		}
	}
	buildVector node1(vec.vecOffset + 1, vec.hiLimit, 0, vec.vecPtr);
	//Create first sub branche
	short result = node1.calculateNextOperation(node1.lowLimit, node1.hiLimit);
	if (result < 0)
	{
		return false;
	}
	mathNode::mathExpressionNode* mathNode1;
	if (vec.vecPtr->operator[](result)->hasNode())
	{
		mathNode1 = vec.vecPtr->operator[](result)->node();
	}
	else
	{
		std::cerr << "Can't create sub node!\n";
		return false;
	}

	node1.vecOffset = result;
	nodeDataInterface_wrapper_access(tgt)->createSubNodes(mathNode1, nullptr);
	if (!buildSubNodes(mathNode1, node1))
	{
		return false;
	}

	return true;
}




bool buildSubNodes(mathNode::mathExpressionNode * target, buildVector vec)
{

	switch (target->type)
	{
	case tree::VARIABLE:
		return true;
			break;
	case tree::OPERATOR:
		return _operator_build(static_cast<mathNode::mathExpressionNode_opr*>(target), vec);
			break;
	case tree::VALUE:
		return true;
		break;
	case tree::FUNCTION:
		return _function_build(static_cast<mathNode::mathExpressionNode_func*>(target), vec);
		break;
	case tree::FUNCTION_TREE:
		return _function_build_tree(static_cast<mathNode::mathExpressionNode_func_tree*>(target), vec);
		break;
	default:
	{
			   std::cerr << "Can't build subnodes, unsopported type\n";
		return false;
	    break;
	}
	}

}
