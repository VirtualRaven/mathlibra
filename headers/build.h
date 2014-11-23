
#include <iostream>
#include "operators.h"
//Contains function for creating an exression tree


struct buildVector
{
	short lowLimit;
	short hiLimit;
	unsigned short vecOffset;
	std::vector<baseToken*>* vecPtr;
	buildVector(short lowLimit, short hiLimit, short vecOffset, std::vector<baseToken*>* vecPtr);
	buildVector();
	//Return types, -1 error, >0 next operator index
	short calculateNextOperation(short lowLimit, short hiLimit);

};

bool buildSubNodes(mathNode::mathExpressionNode * target, buildVector vec);


