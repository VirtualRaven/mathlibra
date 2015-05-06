#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED
#include <iostream>
#include "exception.h"
#include "main.h"
#include "tree_interface.h"

namespace tree
{
	
	class TreeStructOops : public exception
	{
	public:
		TreeStructOops(std::string inf, bool isCritical);
		const char* what();
	};






	node_base * nodeDataInterface_wrapper_access(nodeDataInterface* node);


	class node : public  node_base
	{
	protected:
		node * nodePtr1;
		node * nodePtr2;
	public:

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
		node(const node&);
		node(node&&);
		node& operator=(const node&);
		node& operator=(node&&);
	};
	/*
	 A wrapper for node which implement futher functions to make rootnode serve as an interface for the complete tree
	 The class can be copied but of an tree is asigned to the rootnode the tree will not be copied, instead the new root node and the old will share the same tree,
	 Therfeore any operation that acts upoun the tree itself on a specific root node will apply to all copies of this root node. The last object remaining will automaticly
	 remove the tree.
	 */
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
		void TakeContext(); //Call before operation on tree if several copies exists
		rootNode(const rootNode& sourceNode);
		rootNode& operator=(const rootNode& sourceNode);
		rootNode(rootNode&& sourceNode);
		rootNode& operator=(rootNode&& sourceNode);
	};

	void display_tree(rootNode tree);
}

#endif
