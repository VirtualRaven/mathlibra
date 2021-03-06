/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
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
	this->__make_local_context(&this->__tree);
	this->__tree.TakeContext();
        this->__tree.data->optimize();
}
type* interpreted_func::operator()(interface::type_ptr x)
{
	this->local_mem.set_ignore_const("x",std::move(x), true, true);
	return this->__tree.data->eval();
}
type* interpreted_func::exec(interface::type_ptr x )
{
	return this->operator()(std::move(x));
}
interpreted_func::interpreted_func(tree::node* tree,  memory::memory* mem) :  
local_mem(mem->copy_const()), 
__tree(std::move(*tree))
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
		this->local_mem = func.local_mem;
		this->__prepare_function();
		return *this;
}

interpreted_func::interpreted_func(interpreted_func&& func) : 
	__tree(std::move(func.__tree))
{
	this->__prepare_function();
}

interpreted_func::~interpreted_func()
{

}
}
