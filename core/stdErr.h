/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include <iostream>
#include <sstream>
#include "main.h"
/**
 * err_redirect redirects the cerr. 
 * This class is used to redirect the standard error output to an altenrative stream.
 * Can be used to implement logging of error messages to an file
 */
class err_redirect {

	std::stringstream stream;
	std::streambuf * newBuff;
	std::streambuf * old;


public:

	void toggle();
	err_redirect();
	~err_redirect();
	std::string cerr();



};
