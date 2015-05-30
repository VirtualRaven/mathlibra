
#include <iostream>
#include "core/tokens.h"

//Contains function for creating an exression tree
#include <vector>

struct buildVector
{
	short lowLimit;
	short hiLimit;
	unsigned short vecOffset;
	std::vector<token::baseToken*>* vecPtr;
	buildVector(short lowLimit, short hiLimit, short vecOffset, std::vector<token::baseToken*>* vecPtr);
	buildVector();
	//Return types, -1 error, >0 next operator index
	short calculateNextOperation(short lowLimit, short hiLimit);

};

bool buildSubNodes(tree::nodeDataInterface * target, buildVector vec);


