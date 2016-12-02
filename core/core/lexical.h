/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef LEXICAL_H_INCLUDED
#define LEXICAL_H_INCLUDED
#include "tokens.h"
#include <vector>
#include "modules/memory.h"
#include "modules/functions.h"
#include "modules/operators.h"
#include "core/token_list.h"

typedef std::vector<token::baseToken*> tvec;
struct state
{
		size_t startPos;
		tvec tokens;
                token_list::token_list items;

};

state lexical(const char * expr,
	      size_t expr_len,
	      memory::memory*, 
	      math_func::function_interface*,
	      operators::operators_interface* );

#endif
