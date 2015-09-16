#include <iostream>
#include <sstream>
#include "main.h"
/**
 * err_redirect redirects the cerr. 
 * This class is used to redirect the standard error output to an altenrative stream.
 * Can be used to implement logging of error messages to an file
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
