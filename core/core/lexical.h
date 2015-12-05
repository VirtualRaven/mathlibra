#ifndef LEXICAL_H_INCLUDED
#define LEXICAL_H_INCLUDED
#include "tokens.h"
#include <vector>
#include "modules/memory.h"
#include "modules/functions.h"
#include "modules/operators.h"

typedef std::vector<token::baseToken*> tvec;

struct state
{
		size_t startPos;
		tvec tokens;

};

state lexical(const char * expr,
	      size_t expr_len,
	      memory::memory*, 
	      math_func::function_interface*,
	      operators::operators_interface* );

#endif
