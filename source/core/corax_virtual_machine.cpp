#include "corax_virtual_machine.h"

namespace CoraxVM
{

         const  instruction    instruction_set::LDI =   0x1000u;
         const  instruction    instruction_set::LD  =   0x2000u;
         const  instruction    instruction_set::ST  =   0x3000u;
         const  instruction    instruction_set::PUSH=   0x4000u;
         const  instruction    instruction_set::POP =   0x5000u;
         const  instruction    instruction_set::CALL=   0x6000u;


         const instruction instruction_flags::FLAG1   = 0x100u;
         const instruction instruction_flags::FLAG2   = 0x200u;
         const instruction instruction_flags::FLAG3   = 0x400u;
         const instruction instruction_flags::FLAG4   = 0x800u;


    coraxOops::coraxOops(std::string inf)
        {
             info=inf;
        };

	const char* coraxOops::what()
	{
		return "coraxVM Exception";
	}



        CoraxVM::symbol::symbol(byte nr_,std::string name_,type type_):
          value(nr_),
          name(name_),
          symbol_type(type_)
            {};


       short symbol_table::_find(byte val,symbol::type symbol_type)
        {

            if(symbol_type== symbol::type::VARIABLE)
            {
                for(short i =0; i< this->_variables.size(); i++ )
                {
                    if(this->_variables[i].value ==  val)
                    {
                        return i;
                    }
                }
                return -1;
            }
            else
            {
                for(short i =0; i< this->_funcs.size(); i++ )
                {
                    if(this->_funcs[i].value ==  val)
                    {
                        return i;
                    }
                }
                return -1;
            }
        }

        short symbol_table::_find(std::string name, symbol::type symbol_type)
        {

            if(symbol_type== symbol::type::VARIABLE)
            {
                for(short i =0; i< this->_variables.size(); i++ )
                {
                    if(this->_variables[i].name ==  name)
                    {
                        return i;
                    }
                }
                return -1;
            }
            else
            {
                for(short i =0; i< this->_funcs.size(); i++ )
                {
                    if(this->_funcs[i].name ==  name)
                    {
                        return i;
                    }
                }
                return -1;
            }
        }

        byte symbol_table::add(std::string name, symbol::type symbol_type)
        {
            short val = _find(name,symbol_type);
            if(val == -1)
            {
                if(symbol_type== symbol::VARIABLE)
                {
                    _variables.push_back(symbol(this->_variables.size()+1,name,symbol_type));
                    return this->_variables.size();
                }
                else
                {

                    _funcs.push_back(symbol(this->_funcs.size()+1,name,symbol_type));
                    return this->_funcs.size();
                }
            }
            return val; //Symbol already defined, returning old symbol number
        }
        std::string symbol_table::get(byte num,symbol::type symbol_type)
        {
            short val = _find(num,symbol_type);
            if(val == -1)
            {
                throw coraxOops("Reference to undefined symbol");
            }
            else if(symbol_type == symbol::VARIABLE)
            {
                return _variables[val].name;
            }
            else
            {
                return _funcs[val].name;
            }
        }





        void corax::setOperator(operators::operators_interface* operators)
        {
            corax::_operators = operators;
        }

        void corax::setMemory(memory* mem)
        {
            corax::_mem = mem;
        }

        void corax::setFunction(math_func::function_interface* functions)
        {
            corax::_functions = functions;
        }

        number_type corax::run(corax_program * prgm)
        {
            return 0;
        }

        corax::corax() :
            _operators(nullptr),
            _mem(nullptr),
            _functions(nullptr)
        {};

        corax::corax(operators::operators_interface* operators,memory* mem,math_func::function_interface* functions) :
            _operators(operators),
            _mem(mem),
            _functions(functions)
        {};

        corax::~corax()
        {

        }


}

