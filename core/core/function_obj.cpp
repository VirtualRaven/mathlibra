#include "function_obj.h"
#include "mathNode.h"
#include "core/type_helper.h"
namespace function_obj
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
		this->local_mem.set(var, interface::type_ptr(this->global_mem->get(var)->copy()), true, true);
	}
	this->__make_local_context(&this->__tree);
}
type* interpreted_func::operator()(type* x)
{
	this->local_mem.set_ignore_const("x", interface::type_ptr(x), true, true);
	this->__tree.TakeContext();
	return this->__tree.data->eval();
}
type* interpreted_func::exec(type* x )
{
	return this->operator()(x);
}
interpreted_func::interpreted_func(tree::node* tree,  memory::memory* mem) :  global_mem(mem), __tree(std::move(*tree))
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
