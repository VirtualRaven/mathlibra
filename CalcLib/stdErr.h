#include <iostream>
#include <sstream>
#include "main.h"
/**
 * err_redirect redirects the cerr. 
 */
class err_redirect {

	std::stringstream stream;
	std::streambuf * newBuff;
	std::streambuf * old;


public:

	void toggle();
	err_redirect();
	~err_redirect();
	std::string cerr();



};
