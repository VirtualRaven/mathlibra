#include <iostream>
#include <sstream>

class err_redirect {

	

public:
	std::streambuf * old;
	std::streambuf * newBuff;
	std::stringstream stream;

	err_redirect() 
	{ 
		newBuff = stream.rdbuf();
		old = std::cerr.rdbuf(newBuff);
		if (std::cerr.fail())
		{
			std::cout << "Failed to redirect!\n";
		}

	}

	~err_redirect() 
	{
		std::cerr.rdbuf(old);
	}
	std::string cerr()
	{
		
		return stream.str();

	}

	
};