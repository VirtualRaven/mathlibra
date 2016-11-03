/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED
#include <vector>
#include <string>
using std::vector;
using std::string;

enum struct RETURN_VAL : short { SUCCESS =0, MEM_ERROR=1, EXCEPTION=2, CRITICAL_EXCEPTION=3,OTHER=4 }; 
RETURN_VAL  runTest(std::string,vector<string> testExpression);
#endif
