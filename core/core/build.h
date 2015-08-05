
#include <iostream>
#include "core/tokens.h"

//Contains function for creating an exression tree
#include <vector>

struct buildVector
{
	size_t lowLimit;
	size_t hiLimit;
	size_t vecOffset;
	std::vector<token::baseToken*>* vecPtr;
	buildVector(size_t lowLimit, size_t hiLimit, size_t vecOffset, std::vector<token::baseToken*>* vecPtr);
	buildVector();
	//Return types, -1 error, >0 next operator index
	size_t calculateNextOperation(size_t lowLimit, size_t hiLimit);

};

bool buildSubNodes(tree::nodeDataInterface * target, buildVector vec);


