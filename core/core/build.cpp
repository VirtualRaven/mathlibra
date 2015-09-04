#include "build.h"
#include "ptr_protect.h"
using tree::rootNode;

struct build_exception : exception
{
	const char* what()
	{
		return "expression interpetation failure";
	}
	
	build_exception(std::string inf)
	{
		this->info = inf;
		this->_isCritical = false;
	}

};



buildVector::buildVector(size_t lowLimit, size_t hiLimit, size_t vecOffset, std::vector<token::baseToken*>* vecPtr)
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
	size_t buildVector::calculateNextOperation(size_t lowLimit, size_t hiLimit)
	{
		size_t lowWheight = 9999;
		size_t nextOperation = -2;
		size_t lenght = 0;
		size_t values = 0;
		token::baseToken* basePtr = nullptr;
		for (size_t i = lowLimit; i < hiLimit; i++)
		{
			 
			basePtr = vecPtr->operator[](i);
			if (basePtr->type == tree::OPERATOR && static_cast<token::operatorToken*>(basePtr)->baseWheight <= lowWheight)  //Check for next least important operator
			{
				lowWheight = static_cast<token::operatorToken*>(basePtr)->baseWheight;
				nextOperation = i;
				lenght++;
			}
			else if (basePtr->type == tree::FUNCTION && static_cast<token::funcToken*>(basePtr)->baseWheight <= lowWheight)  //Check for next least important operator
			{
				lowWheight = static_cast<token::funcToken*>(basePtr)->baseWheight;
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
			for (size_t i = lowLimit; i < hiLimit; i++)
			{
				basePtr = vecPtr->operator[](i);
				if (basePtr->type == tree::VALUE || basePtr->type == tree::VARIABLE)
				{
					return i;
				}
			}
		}


		//Find error if program comes here
		//std::cerr << "-[ Unknown error ]\n";
		throw build_exception("Unknow error in syntax interpetation");
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

			size_t result = node1.calculateNextOperation(node1.lowLimit, node1.hiLimit);
			if(result < 0)
			{
				return false;
			}

			tree::nodeDataInterface* mathNode1=nullptr;
			ptr_protect<nodeDataInterface*, false> mathNode1_guard(mathNode1);
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

			tree::nodeDataInterface* mathNode2=nullptr;
			ptr_protect<nodeDataInterface*, false> mathNode2_guard(mathNode2);
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




			if (tgt->ptr == nullptr)
			{
				tgt->type = tree::DUMMY;
			}
			auto tmp = static_cast<tree::node*>(nodeDataInterface_wrapper_access(tgt));
			tmp->createSubNodes(mathNode1, mathNode2);
			
			mathNode1_guard.release();
			mathNode2_guard.release();
			
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
	for (size_t i = vec.lowLimit; i < vec.vecOffset; i++)
	{
		if (vec.vecPtr->operator[](i)->type != tree::PARENTHESES)
		{
			std::cerr << "Vital token on lefhand side of function in build vector!\n";
			return false;
		}
	}
	buildVector node1(vec.vecOffset + 1, vec.hiLimit, 0, vec.vecPtr);
	//Create first sub branche
	size_t result = node1.calculateNextOperation(node1.lowLimit, node1.hiLimit);
	if (result < 0)
	{
		return false;
	}
	tree::nodeDataInterface* mathNode1;
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
	auto tmp = static_cast<tree::node*>(nodeDataInterface_wrapper_access(tgt));

	tmp->createSubNodes(mathNode1,nullptr);
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
	for (unsigned int i = vec.lowLimit; i < vec.vecOffset; i++)
	{
		if (vec.vecPtr->operator[](i)->type != tree::PARENTHESES)
		{
			std::cerr << "Vital token on lefhand side of function in build vector!\n";
			return false;
		}
	}
	buildVector node1(vec.vecOffset + 1, vec.hiLimit, 0, vec.vecPtr);
	//Create first sub branche
	size_t result = node1.calculateNextOperation(node1.lowLimit, node1.hiLimit);
	if (result < 0)
	{
		return false;
	}
	tree::nodeDataInterface* mathNode1;
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
	auto tmp = static_cast<tree::node*>(nodeDataInterface_wrapper_access(tgt));
	tmp->createSubNodes(mathNode1, nullptr);
	if (!buildSubNodes(mathNode1, node1))
	{
		return false;
	}

	return true;
}




bool buildSubNodes(tree::nodeDataInterface * target, buildVector vec)
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