/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "lexical.h"

namespace __internal
{
	typedef std::stack<token::parenthesesToken*> pstack;


	struct i_state
	{
		size_t* operWheight;
		size_t* i;
		size_t* lowestWheight;
		size_t* const lenght;
		size_t* start;
	};
	
	//Util
	void pushMulti(tvec&,operators::operators_interface*,i_state&);
	void pushValue(tvec&,i_state&,double val);
	void updateStartPoint(tvec&,i_state&,size_t w);
	//Parsers
	void parse_string(const char * expr, tvec& tokens, i_state& s);
	void parse_parantheses1(tvec&,i_state&, pstack&, operators::operators_interface*);
	void parse_parantheses2(tvec&,i_state&, pstack&);
	void parse_number(const char*,tvec&, i_state&,operators::operators_interface*);  
	void parse_opr( tvec&,i_state&,operators::operators_interface*);
	void parse_func(std::string ,tvec&,i_state&,math_func::function_interface*);
	void parse_var(std::string, tvec&,i_state&,math_func::function_interface*, memory::memory*);	
}
