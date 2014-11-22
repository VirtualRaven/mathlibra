#include "tree.h"

const char* TreeStructOops::what()
{
	return "TreeStructure Exception";
}


void nodeDataInterface::bind(node * context)
{

}

 void nodeDataInterface::destroy()
{


}

 node * nodeDataInterface_wrapper_access(nodeDataInterface* node)
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
node * node::sub1()
{
	return nodePtr1;
}
node * node::sub2()
{
	return nodePtr2;
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
{
	this->data = nullptr; //IMPORTANT!!!!!!!!!!!!!!
	this->nodePtr1 = nullptr;
	this->nodePtr2 = nullptr;
}

node::node(nodeDataInterface* val)
{
	this->data = val;
	this->nodePtr1 = nullptr;
	this->nodePtr2 = nullptr;
}
node::~node()
{

}

void node::set(nodeDataInterface* data)
{
	this->data = data;
	this->data->bind(this);
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

			std::cerr << "Destroyed root node\n";

			delete copies;
			copies = nullptr;

			std::cerr << "Destroyed copy index node\n";

		}
	}
	else
	{

		
		throw TreeStructOops("Uninitilized counter of copies\n");
	}
}
void rootNode::_copy(const rootNode& sourceNode)
{
	this->data = sourceNode.data;
	this->nodePtr1 = sourceNode.nodePtr1;
	this->nodePtr2 = sourceNode.nodePtr2;
	if (sourceNode.copies == nullptr)
	{
	
		throw TreeStructOops("Unitilized copies pointer in constructor\n");
	}
	this->copies = sourceNode.copies;
	(*copies)++;
	if (this->data != nullptr)
	{
		this->TakeContext();
	}
	else
	{
		throw TreeStructOops("Failed to copy Treestucture, data pointer empty");
	}
}


//PUBLIC FUNTIONS

rootNode::~rootNode()
{
	_empty();
}
rootNode::rootNode()
{
	this->data = nullptr;
	this->nodePtr1 = nullptr;
	this->nodePtr2 = nullptr;
	copies = new int;
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
		throw TreeStructOops("Cant get context, data pointer empty");
	}
}
rootNode::rootNode(const rootNode& sourceNode)
{
	_copy(sourceNode);
}

void rootNode::operator=(const rootNode& sourceNode)
{
	_empty();
	_copy(sourceNode);

}
