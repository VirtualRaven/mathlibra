#include "function_obj.h"

namespace math_func
{
	
	inline void __switch_mem(tree::node* next, memory::memory* mem)
	{
		if (next->data->type == tree::VARIABLE)
		{
			auto tmp =static_cast<mathNode::mathExpressionNode_variable*>(next->data);
			tmp->set_mem_provider(mem);
		}
	}


void   interpreted_func::__make_local_context(tree::node* current)
{
	auto sub1 = current->sub1();
	auto sub2 = current->sub2();
		if (sub1 == nullptr && sub2 == nullptr)
		{
			__switch_mem(current, &this->local_mem);
			return;
		}
		else
		{
			if (sub1 != nullptr)
			{
				this->__make_local_context(sub1);
			}
			if (sub2 != nullptr)
			{
				this->__make_local_context(sub2);
			}
		}
		return;
}

void interpreted_func::__prepare_function()
{
	for (auto var : this->global_mem->allVars())
	{
		this->local_mem.set(var, this->global_mem->get(var), true, true);
	}
	this->__make_local_context(&this->__tree);
}
double interpreted_func::operator()(double x)
{
	this->local_mem.set("x", x, true, true);
	this->__tree.TakeContext();
	return this->__tree.data->eval();
}
double interpreted_func::exec(double x )
{
	return this->operator()(x);
}
interpreted_func::interpreted_func(tree::node* tree,  memory::memory* mem) : __tree(std::move(*tree)), global_mem(mem)
{
	this->__prepare_function();
}
interpreted_func& interpreted_func::operator=(tree::node* tree)
{
	this->__tree = std::move(*tree);
	this->__prepare_function();
	return *this;
}

interpreted_func& interpreted_func::operator=(interpreted_func&& func)
{
		this->__tree = std::move(func.__tree);
		this->global_mem = func.global_mem;
		this->local_mem = func.local_mem;
		this->__prepare_function();
		return *this;
}

interpreted_func::interpreted_func(interpreted_func&& func) : __tree(std::move(func.__tree))
{
	this->__prepare_function();
}

interpreted_func::~interpreted_func()
{

}
}