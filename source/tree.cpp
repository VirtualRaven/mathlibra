#include "tree.h"
#include <vector>

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

struct PtrPair
{
	node * x;
	node *y;
	PtrPair(node * x, node *y)
	{
		this->x = x;
		this->y = y;
	}
};
/*
void display_tree(rootNode tree)
{
	typedef std::vector<PtrPair> levelVec;
	std::vector<levelVec> treeVec;
	levelVec level1Vec;
	level1Vec.push_back(PtrPair(tree.sub1(), tree.sub2()));
	treeVec.push_back(level1Vec);
	
	do
	{
		levelVec currentVec = treeVec.back();
		levelVec nextVec;
		for (int i = 0; i < currentVec.size(); i++)
		{
			
			PtrPair xy(currentVec[i].x->sub1,currentVec[i].y->sub2);
			if (currentVec[i].x)
			{

			}
			nextVec.push_back(xy);
		}
		treeVec.push_back(nextVec);
	}


}*/