#include "tree.h"
#include <vector>
namespace tree
{

	TreeStructOops::TreeStructOops(std::string inf, bool isCritical): exception(inf, isCritical){}

	const char* TreeStructOops::what()
	{
		return "TreeStructure Exception";
	}


	node_base * nodeDataInterface_wrapper_access(nodeDataInterface* node)
	{
		return node->wrapperNode;
	}

	/* bool nodeDataInterface::buildSub(buildVector<double> vec)
	{
	return true;
	}
	*/

	/*
		node
		*/

	void node::createSubNodes(nodeDataInterface* data1, nodeDataInterface* data2) // Creats one or two sub nodes
	{
		this->deleteSubNodes();
		this->nodePtr1 = new node;
		this->nodePtr1->data = data1;
		this->nodePtr1->data->bind(this->nodePtr1);

		if (data2 != nullptr)
		{
			this->nodePtr2 = new node;
			this->nodePtr2->data = data2;
			this->nodePtr2->data->bind(this->nodePtr2);
		}
	}
#ifdef STRUCTUAL_INTEGRITY_TEST
	void node::integrityTest(int level )
	{
		std::cerr << "Testing level " << level << "\nNodePtr1: " << nodePtr1 << "\nNodePtr2: " << nodePtr2 << "\n";

		std::cerr << "\n\n";
		if (nodePtr1 != nullptr)
		{
			nodePtr1->integrityTest(level + 1);
		}
		if (nodePtr2 != nullptr)
		{
			nodePtr2->integrityTest(level + 1);
		}

	}
#endif
	node * node::sub1()
	{
		return nodePtr1;
	}
        node* node::take_ownership_sub1()
        {
            auto tmp = nodePtr1;
            nodePtr1=nullptr;
            return tmp;
        }
	node * node::sub2()
	{
		return nodePtr2;
	}
        node* node::take_ownership_sub2()
        {
            auto tmp = nodePtr2;
            nodePtr2=nullptr;
            return tmp;
        }
	void node::deleteSubNodes()
	{
		if (this->nodePtr1)
		{
			this->nodePtr1->deleteSubNodes();
			this->nodePtr1->data->destroy();
			delete this->nodePtr1;
		}
		if (this->nodePtr2)
		{
			this->nodePtr2->deleteSubNodes();
			this->nodePtr2->data->destroy();
			delete this->nodePtr2;
		}
		this->nodePtr1 = nullptr;
		this->nodePtr2 = nullptr;

	}

	node::node()
		:nodePtr1(nullptr),
		nodePtr2(nullptr)
		
		{ data=nullptr; }

	node::node(nodeDataInterface* val)
		: nodePtr1(nullptr),
		nodePtr2(nullptr)
	{
		data = val;
	}

	node::node(node&& other) :
		nodePtr1(other.nodePtr1),
		nodePtr2(other.nodePtr2)
		
	{
		data = other.data;
		other.nodePtr1 = nullptr;
		other.nodePtr2 = nullptr;
		other.data = nullptr;
	}
	node::~node()
	{

	}
	void node::set(nodeDataInterface* data)
	{
		this->data = data;
		this->data->bind(this);
	}

	node::node(const node&)
		:nodePtr1(nullptr),
		nodePtr2(nullptr)
		
	{
		data = nullptr;
		throw TreeStructOops("bare node can not be copied",true);
	}
	node& node::operator=(const node&)
	{
		throw TreeStructOops("bare node can not be copied",true);
	}
	node& node::operator=(node&& other)
	{
		this->nodePtr1 = other.nodePtr1;
		this->nodePtr2 = other.nodePtr2;
		this->data = other.data;
		other.nodePtr1 = nullptr;
		other.nodePtr2 = nullptr;
		other.data = nullptr;
		return *this;
	}

	/*



		rootNode


		*/
	//PRIVATE

	void rootNode::_empty()
	{
		if (copies != nullptr)
		{
			(*copies)--;
			if (*copies == 0)
			{
				this->deleteSubNodes();
				if (data != nullptr)
				{
					this->data->destroy();
				}

				this->data = nullptr;
#ifdef SYNTAX_TREE_DEBUG_INFO
				std::cerr << "Destroyed root node\n";
#endif
				delete copies;
				copies = nullptr;
#ifdef SYNTAX_TREE_DEBUG_INFO
				std::cerr << "Destroyed copy index node\n";
#endif
			}
		}
		else
		{


			throw TreeStructOops("Uninitilized counter of copies\n",true);
		}
	}
	void rootNode::_copy(const rootNode& sourceNode)
	{
		this->data = sourceNode.data;
		this->nodePtr1 = sourceNode.nodePtr1;
		this->nodePtr2 = sourceNode.nodePtr2;
		if (sourceNode.copies == nullptr)
		{

			throw TreeStructOops("Unitilized copies pointer in constructor\n", true);
		}
		this->copies = sourceNode.copies;
		(*copies)++;
		if (this->data != nullptr)
		{
			this->TakeContext();
		}
		else
		{
			throw TreeStructOops("Failed to copy Treestucture, data pointer empty",true);
		}
	}


	//PUBLIC FUNTIONS

	rootNode::~rootNode()
	{
		_empty();
	}
	rootNode::rootNode()
		:node(),
		copies(new int)
	{

		*copies = 1;
	}
	void rootNode::TakeContext()
	{
		if (this->data != nullptr)
		{
			this->data->bind(this);
		}
		else
		{
			throw TreeStructOops("Cant get context, data pointer empty",true);
		}
	}
	rootNode::rootNode(const rootNode& sourceNode)
	{
		_copy(sourceNode);
	}

	rootNode& rootNode::operator=(const rootNode& sourceNode)
	{
		_empty();
		_copy(sourceNode);
		return *this;

	}
	rootNode::rootNode(rootNode&& sourceNode) :
		node(std::move(sourceNode)),
		copies(sourceNode.copies)
	{
		this->copies = sourceNode.copies;
		sourceNode.copies = new int;
		*(sourceNode.copies) = 1;

	}
	
	rootNode& rootNode::operator=(rootNode&& sourceNode)
	{
		
		this->_empty();  //release our ownership of the current data in the node

		this->nodePtr1 = sourceNode.nodePtr1;
		this->nodePtr2 = sourceNode.nodePtr2;
		this->data = sourceNode.data;
		sourceNode.nodePtr1 = nullptr;
		sourceNode.nodePtr2 = nullptr;
		sourceNode.data = nullptr;
		this->copies = sourceNode.copies;
		sourceNode.copies = new int;
		*(sourceNode.copies) = 1;
		return *this;
	}

	struct PtrPair
	{
		node * x;
		node *y;
		PtrPair(node * _x, node *_y)
			:x(_x),
			y(_y)
		{}
	};



	void node::raiseException(const char* inf)
	{
		throw TreeStructOops(inf, false);
	}
	//Breadth first function for displaying the syntax tree under the node_base
	std::stack<tree::node_base*> node::getArgs()
	{
		node_base * current = this;
		node_base * next = nullptr;
		std::stack<node_base*> args;

		while (current != nullptr)
		{
			//Check second branch 
			if (current->sub2() != nullptr)
			{
				if (current->sub2()->data->type == tree::DUMMY)
				{
					next = current->sub2();
				}
				else
				{
					args.push(current->sub2());
				}
			}

			//Check first branch
			if (current->sub1() != nullptr)
			{
				if (current->sub1()->data->type == tree::DUMMY)
				{
					next = current->sub1();
				}
				else
				{
					args.push(current->sub1());
				}
			}

			current = next;
			next = nullptr;
		}
		return args;
	}
	rootNode& rootNode::operator=(node&& sourceNode)
	{
		this->_empty();
		this->copies = new int;
		*this->copies = 1;
		this->nodePtr1 = sourceNode.sub1();
		this->nodePtr2 = sourceNode.sub2();
		this->data = sourceNode.data;
		this->TakeContext();
		sourceNode.data = nullptr;
		return *this;
	}

	rootNode::rootNode(node&& sourceNode): node(), copies(new int)
	{
		*(this->copies) = 1;
		this->nodePtr1 = sourceNode.nodePtr1;
		sourceNode.nodePtr1 = nullptr;
		this->nodePtr2 = sourceNode.nodePtr2;
		sourceNode.nodePtr2 = nullptr;
		this->data = sourceNode.data;
		this->TakeContext();
		sourceNode.data = nullptr;
	}
}
