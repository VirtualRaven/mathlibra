#include "build.h"
#include "ptr_protect.h"
#include "exception_helper.h"
using tree::rootNode;

template<EXCEPTION T> void buildOops()
{
	__mathlibra__raise<T,BUILD>();
}

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
	size_t buildVector::calculateNextOperation(size_t _lowLimit, size_t _hiLimit)
	{
		size_t lowWheight = 9999;
		size_t nextOperation = 0;
		size_t lenght = 0;
		size_t values = 0;
		token::baseToken* basePtr = nullptr;
		for (size_t i = _lowLimit; i < _hiLimit; i++)
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
			for (size_t i = _lowLimit; i < _hiLimit; i++)
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
		buildOops<BUILD_UNKNOWN_ERROR>();
		return 0; 
	}

bool _operator_build(mathNode::mathExpressionNode_opr * tgt, buildVector vec)
		{

			
			buildVector node1(0,0,0,vec.vecPtr);
			buildVector node2(0,0,0,vec.vecPtr);

			//Simple syntax checking for the operators. Implement more advanced checking
			if(vec.vecOffset <= 0 )
			{
				buildOops<SYNTAX_EXPECTED_LH_OPERAND>();
				return false;

			}
			else if (vec.vecPtr->operator[](vec.vecOffset - 1)->type == tokenType::OPERATOR) //Won't work if user writes any formating token before the operator
			{
				buildOops<SYNTAX_DUPLICATED_OPERATORS_LH>();
				return false;
			}
			else if( vec.vecOffset >= vec.vecPtr->size()-1 )
			{
				buildOops<SYNTAX_EXPECTED_RH_OPERAND>();
				return false;
			}
			else if(vec.vecPtr->operator[](vec.vecOffset+1)->type == tokenType::OPERATOR) //Won't work if user writes any formating token before the operator
			{
				buildOops<SYNTAX_DUPLICATED_OPERATORS_RH>();
				return false;
			}


			node1.lowLimit=vec.lowLimit;
			node1.hiLimit=vec.vecOffset;

			//Create first sub branche

			size_t result = node1.calculateNextOperation(node1.lowLimit, node1.hiLimit);

			if(!vec.vecPtr->operator[](result)->hasNode())
			{
				std::cerr << "Can't create sub node!\n";
				return false;
			}

			ptr_protect<nodeDataInterface*, false> mathNode1_guard(vec.vecPtr->operator[](result)->node());

			node1.vecOffset=result;

			node2.lowLimit = vec.vecOffset + 1;
			node2.hiLimit = vec.hiLimit;
			result =	node2.calculateNextOperation(node2.lowLimit,node2.hiLimit);

			if (!vec.vecPtr->operator[](result)->hasNode())
			{
				std::cerr << "Can't create sub node!\n";
				return false;
			}
			ptr_protect<nodeDataInterface*, false> mathNode2_guard(vec.vecPtr->operator[](result)->node());
			node2.vecOffset=result;

			if (tgt->ptr == nullptr)
			{
				tgt->type = tree::DUMMY;
			}
			auto tmp = static_cast<tree::node*>(nodeDataInterface_wrapper_access(tgt));
			mathNode1_guard.release();
			mathNode2_guard.release();
			tmp->createSubNodes(mathNode1_guard.ptr(), mathNode2_guard.ptr());
			if( !buildSubNodes(mathNode1_guard.ptr()  ,node1) || 	!buildSubNodes(mathNode2_guard.ptr() ,node2))
			{
				return false;
			}
			return true;
		}



bool _function_build_tree(mathNode::mathExpressionNode_func_tree * tgt, buildVector vec)
{	
	if (vec.vecOffset >= vec.vecPtr->size() - 1 || ((vec.vecOffset == vec.vecPtr->size() - 2) && vec.vecPtr->operator[](vec.vecOffset+1)->type ==tree::PARENTHESES ))
	{
		buildOops<SYNTAX_EXPECTED_FUNC_ARG>();

	}

	
/*	else if (vec.vecPtr->operator[](vec.vecOffset + 1)->type != tree::PARENTHESES) //won't work if user writes any formating token before the operator
	{
		std::cerr << "excpected parentheses after function name";
		return false;
	}*/
	for (unsigned int i = vec.lowLimit; i < vec.vecOffset; i++)
	{
		if (vec.vecPtr->operator[](i)->type != tree::PARENTHESES)
		{
			std::cerr << "vital token on lefhand side of function in build vector!\n";
			return false;
		}
	}
	buildVector node1(vec.vecOffset + 1, vec.hiLimit, 0, vec.vecPtr);
	//create first sub branche
	    size_t result = node1.calculateNextOperation(node1.lowLimit, node1.hiLimit);
	    if (!vec.vecPtr->operator[](result)->hasNode())
	    {
		std::cerr << "can't create sub node!\n";
		return false;
	    }
	    ptr_protect<tree::nodeDataInterface*,false> mathNode1_guard(vec.vecPtr->operator[](result)->node());
            node1.vecOffset = result;
	    auto tmp = static_cast<tree::node*>(nodeDataInterface_wrapper_access(tgt));
            mathNode1_guard.release();
	    tmp->createSubNodes(mathNode1_guard.ptr(), nullptr);
	    if (!buildSubNodes(mathNode1_guard.ptr(), node1))
	    {
		return false;
	    }
            else
            {
                return true;
            }
        
	

}


bool _function_build_user(mathNode::mathExpressionNode_func_user * tgt, buildVector vec)
{
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
	if(node1.lowLimit != node1.hiLimit)
        {
            size_t result = node1.calculateNextOperation(node1.lowLimit, node1.hiLimit);
	
	    if (!vec.vecPtr->operator[](result)->hasNode())
	    {
		std::cerr << "Can't create sub node!\n";
		return false;
	    }
	    ptr_protect<nodeDataInterface*, false> mathNode1_guard(vec.vecPtr->operator[](result)->node());
            node1.vecOffset = result;
	    auto tmp = static_cast<tree::node*>(nodeDataInterface_wrapper_access(tgt));

            mathNode1_guard.release();
	    tmp->createSubNodes(mathNode1_guard.ptr() ,nullptr);
	    if (!buildSubNodes(mathNode1_guard.ptr(), node1))
	    {
		return false;
	    }
	    else
            {
                return true;
            }
        }
        else return true;
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
        case tree::FUNCTION_TREE:
		return _function_build_tree(static_cast<mathNode::mathExpressionNode_func_tree*>(target), vec);
		break;
        case tree::FUNCTION_USER:
                return _function_build_user (static_cast<mathNode::mathExpressionNode_func_user*>(target),vec);
                break;
                    
	default:
	{
			   std::cerr << "Can't build subnodes, unsopported type\n";
		return false;
	    break;
	}
	}

}
