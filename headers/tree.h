#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED
#include <iostream>
#include "main.h"
#include "exception.h"

class node;



class TreeStructOops : public exception
{
public:
	TreeStructOops(std::string message)
	{
		this->info = message;
	}
	const char* what();
};

enum tokenType
{
	UNKNOWN =0,
	VARIABLE = 1,
	FUNCTION = 2,
	OPERATOR = 3,
	VALUE = 4,
	PARENTHESES = 5
};



class nodeDataInterface
{
protected:
	node * wrapperNode;
public:
	friend  node * nodeDataInterface_wrapper_access(nodeDataInterface* node);
	tokenType type;
	virtual void bind(node * context);
	virtual void destroy();
	
	 virtual double eval()
	{
		return 0;
	}
	 void _createSubNodes_in_wrapper(nodeDataInterface* data1, nodeDataInterface* data2 = nullptr);
	
};

node * nodeDataInterface_wrapper_access(nodeDataInterface* node);


class node
{
protected:
	node * nodePtr1;
	node * nodePtr2;
public:

	nodeDataInterface* data;
	void createSubNodes(nodeDataInterface* data1, nodeDataInterface* data2 = nullptr); // Creats one or two sub nodes
#ifdef  STRUCTUAL_INTEGRITY_TEST
	void integrityTest(int level = 0);
#endif
	node * sub1();
	node * sub2();
	void deleteSubNodes();
	node();
	node(nodeDataInterface* val);
	~node();
	void set(nodeDataInterface* data);
};

class rootNode : public node
	/* Implements memmory managment and copy functionality */
{
private:
	int * copies;
	void _empty();
	void _copy(const rootNode& sourceNode);

public:

	~rootNode();
	rootNode();
	void TakeContext();
	rootNode(const rootNode& sourceNode);
	void operator=(const rootNode& sourceNode);
};

void display_tree(rootNode tree);


#endif
