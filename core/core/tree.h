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

	/**
	 *Implements the functionality of node_base. 
	 *The interface is further extended with functions for deletion and creation of new child branches.
	 *node also contains move constructors.
	 */
	class rootNode;
	class node : public  node_base
	{
	protected:
		node * nodePtr1;
		node * nodePtr2;
		friend rootNode;
	public:

		void createSubNodes(nodeDataInterface* data1, nodeDataInterface* data2 = nullptr); /**< Construct one or two sub nodes to the current node. @param data1 Data to add to the first child branch. @param data2 Data to add to the second child branch, if data2 == nullptr no second child-branch will be created. */
#ifdef  STRUCTUAL_INTEGRITY_TEST
		void integrityTest(int level = 0);
#endif
		node * sub1();
		node * sub2();
                node * take_ownership_sub1();
                node * take_ownership_sub2();
		std::stack<tree::node_base*> getArgs();
		void raiseException(const char * inf);
		void deleteSubNodes(); /**< Destroy child branches. Delete the data of any child branches and then remove the branch. */
		node();
		node(nodeDataInterface* val);
		virtual ~node();
		void set(nodeDataInterface* data); /**< Sets the data of the node. @param data A pointer to the data to be added.*/
		node(const node&);
		node(node&&); /**< Move constructor. */
		node& operator=(const node&);
		node& operator=(node&&);/**< Move constructor. */
	};
	/**
	 * Extension of node.
	 * A wrapper for node which implement further functions to make rootnode serve as an interface for the complete tree.
	 * The class implements copy constructors and assignment operators. .
	 */
	class rootNode : public node
		/* Implements memmory managment and copy functionality */
	{
	private:
		int * copies;
		void _empty();
		void _copy(const rootNode& sourceNode);

	public:

		~rootNode(); /**< Destructor. The destructor of an rootNode deletes the complete tree upon loosing scope. The default behaviour of the node is to just destroy itself, */
		rootNode();
		void TakeContext(); /**<Synch pointers. calls bind on the trees data members. @note Several different rootNodes can point to the same tree, so to make sure that the trees data points to the right root node a call to bind is required which TakeContext does. @see nodeDataInterface::bind */
		rootNode(const rootNode& sourceNode);
		rootNode& operator=(const rootNode& sourceNode);
		rootNode(rootNode&& sourceNode);
		rootNode& operator=(rootNode&& sourceNode);
		rootNode& operator=(node&& sourceNode);
		rootNode(node&& sourceNode);
	};

	void display_tree(rootNode tree);
}

#endif
