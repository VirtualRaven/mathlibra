/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
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


