/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#pragma once
#include <utility>
#include <chrono>
#ifdef LIB_F_WIN

#endif

/**
 *  @file profiling.h contains function to calculate function execution times.
 */

 /**
  *  func_profile messures time taken to execute function.
  * @param function A function to measure execution time.
  * @param args A list of arguments to be provided to the function upon execution.
  * @tparam TEST_LENGHT A integer representing the number of time to iterate func.
  * @tparam func The type of function.
  * @tparam argN The argument types accepted by function.  
  * @return double representing the time taken to iterate TEST_LENGHT times over function.
  */
template<unsigned int TEST_LENGHT,typename func, typename... argN> double func_profile(func function,argN... args )
{
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	start = std::chrono::high_resolution_clock::now();
	for (unsigned int i = 0; i < TEST_LENGHT; i++)
	{
		function(std::forward<func>(args));
	}
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	return elapsed_seconds.count();
	
}

template<unsigned int TEST_LENGHT, typename func> double func_profile(func function)
{
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	start = std::chrono::high_resolution_clock::now();
	for (unsigned int i = 0; i < TEST_LENGHT; i++)
	{
		function();
	}
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	return elapsed_seconds.count();

}
