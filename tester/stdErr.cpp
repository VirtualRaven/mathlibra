/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "stdErr.h"



	err_redirect::err_redirect()
	:stream(),
	newBuff(stream.rdbuf()),
	old(std::cerr.rdbuf(newBuff))
	{
		if (std::cerr.fail())
		{
			std::cout << "Failed to redirect!\n";
		}
	}
	void err_redirect::toggle()
	{

	}


	err_redirect::~err_redirect()
	{
		std::cerr.rdbuf(old);
	}
	std::string err_redirect::cerr()
	{

		return stream.str();

	}


