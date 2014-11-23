#include <iostream>
#include <sstream>

class err_redirect {



public:
	std::streambuf * old;
	std::streambuf * newBuff;
	std::stringstream stream;

	err_redirect();
	~err_redirect();
	std::string cerr();



};
