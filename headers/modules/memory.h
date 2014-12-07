#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include <string>
#include <vector>
#include "exception.h"
#include "main.h"


struct memoryOops : public exception
{
	memoryOops(std::string info);
	const char* what();
};

struct memoryObject
{
	std::string name;
	number_type value;
	bool constant;
	memoryObject(std::string name, number_type value, bool constant = false);
};

class memory
{
	std::vector<memoryObject> mem;
public:
	number_type get(std::string var);
	bool set(std::string var,number_type value, bool allocateIfNotFound = true, bool constant = false);
	std::vector<std::string> allVars();
	void empty();
};
#endif // MEMORY_H_INCLUDED
