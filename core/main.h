/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#define EXEC_ENV 1
/**
 * Main include file. This file contains global macors and defines.
 */
#if EXEC_ENV == 1
#define SYNTAX_TREE_EXEC
#define EXEC_ENV_STRING "abstract syntax tree"
#elif EXEC_ENV == 2
#define CORAX_VM_EXEC
#define EXEC_ENV_STRING "Corax virtual machine"
#endif


typedef double number_type;
#ifdef DEBUG
#define P_TEST
//#define RUN_TESTS
//#define LEXICAL_ANANALYSIS_DEBUG
//#define SYNTAX_TREE_DEBUG_INFO
//#define STRUCTUAL_INTEGRITY_TEST
//#define DEBUG_CORAX_INS
#endif //DEBUG
namespace util
{

	void buildInfo();
	void help();
	
}

#endif

