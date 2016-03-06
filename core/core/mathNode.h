#ifndef MATHNODE_H_INCLUDED
#define MATHNODE_H_INCLUDED
#include "tree_interface.h"
#include "modules/memory.h"
#include "modules/operators.h"
#include "modules/functions.h"
#include "core/mathNode_interface.h"
#include "main.h"
#include "core/function_obj.h"
struct buildVector;
struct variableToken;
using tree::node_base;
using tree::nodeDataInterface;

/**
 * @namespace mathNode Contains all nodeDataInterface data types
 */

namespace mathNode
{




   class mathExpressionNode_val : public mathNode::mathExpressionNode_val_interface
	{
	public:
	
		mathExpressionNode_val();
		mathExpressionNode_val(interface::type* val);
		virtual ~mathExpressionNode_val();
                interface::type* eval();
		nodeDataInterface*  optimize();
		void bind(node_base * context);
		void destroy();
		bool isPure();

	};

class mathExpressionNode_opr;

	class mathExpressionNode_variable :  public mathNode::mathExpressionNode_variable_interface
	{

	private:
		memory::memory* mem;
                math_func::function_interface* func;
                bool _stack;
		friend mathExpressionNode_opr;
	public:
	        	
                void makeFunction(function_obj::interpreted_func* ptr);
		
                mathExpressionNode_variable(std::string var, memory::memory* mem, bool b,math_func::function_interface * func);
		virtual ~mathExpressionNode_variable();
                interface::type* eval();
		nodeDataInterface* optimize();
		bool is_pushable();
                bool is_undefined();
		bool isPure();
		void set(interface::type_ptr);
		void set_mem_provider(memory::memory* mem_provider);
                memory::memory* get_mem_provider();
		mathExpressionNode_variable(const mathExpressionNode_variable&);
		mathExpressionNode_variable operator=(const mathExpressionNode_variable&);
		void bind(node_base * context);
		void destroy();
	};




	class mathExpressionNode_opr :  public mathNode::mathExpressionNode_opr_interface
	{
	public:

		mathExpressionNode_opr(operators::generic_oper_ptr ptr);
        mathExpressionNode_opr(void);
		virtual  ~mathExpressionNode_opr();
                interface::type* eval();
		nodeDataInterface* optimize();
		bool isPure();
		void bind(node_base * context);
		void destroy();
	};




	class mathExpressionNode_func_tree : public mathNode::mathExpressionNode_func_tree_interface
	{
	public:
		
		mathExpressionNode_func_tree();
		mathExpressionNode_func_tree(funcPtr operation);
		virtual ~mathExpressionNode_func_tree();
                interface::type* eval();
		nodeDataInterface* optimize();
		bool isPure();
		void bind(node_base * context);
		void destroy();
	};
        
        class mathExpressionNode_func_user: public mathNode::mathExpressionNode_func_user_interface
        {
            function_obj::interpreted_func* ptr;
            public:
            virtual ~mathExpressionNode_func_user();
            mathExpressionNode_func_user();
            void bind(node_base * context);
            mathExpressionNode_func_user(function_obj::interpreted_func* ptr);
            interface::type* eval();
	    nodeDataInterface* optimize();
	    bool isPure();
            void destroy();       

        };        



	

}
#endif // MATHNODE_H_INCLUDED
