#include <iostream>
#include <sstream>
#include "main.h"
class err_redirect {

	std::streambuf * old;
	std::streambuf * newBuff;
	std::stringstream stream;

public:
	
	void toggle();
	err_redirect();
	~err_redirect();
	std::string cerr();



};
