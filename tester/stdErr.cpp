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

