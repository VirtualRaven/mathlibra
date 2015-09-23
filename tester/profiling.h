
#pragma once
#include <utility>
#include <chrono>
#ifdef LIB_F_WIN

#endif
typedef std::chrono::milliseconds ms; 

template<unsigned int TEST_LENGHT,typename func, typename... argN> double func_profile(func function,argN... args )
{
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	start = std::chrono::high_resolution_clock::now();
	for (unsigned int i = 0; i < TEST_LENGHT; i++)
	{
		function(std::forward<func>(args...));
	}
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	return std::chrono::duration_cast<ms>( elapsed_seconds).count();

	
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
	return std::chrono::duration_cast<ms>( elapsed_seconds).count();

}
