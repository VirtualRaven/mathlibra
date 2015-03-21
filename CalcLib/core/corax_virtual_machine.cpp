#include "corax_virtual_machine.h"

namespace CoraxVM
{

         const  instruction    instruction_set::LDI =   0x10u;
         const  instruction    instruction_set::LD  =   0x20u;
         const  instruction    instruction_set::ST  =   0x30u;
         const  instruction    instruction_set::PUSH=   0x40u;
         const  instruction    instruction_set::POP =   0x50u;
         const  instruction    instruction_set::CALL=   0x60u;
         const  instruction    instruction_set::SWITCH =   0x70u;


         const instruction instruction_flags::R1   = 0x1u;
         const instruction instruction_flags::R2   = 0x2u;
         const instruction instruction_flags::PR1   = 0x4u;
         const instruction instruction_flags::PR2   = 0x8u;
         const instruction instruction_flags::ARG1 = instruction_flags::R1;
         const instruction instruction_flags::ARG2 = instruction_flags::R2;



    coraxOops::coraxOops(std::string inf)
        {
             info=inf;
        };

	const char* coraxOops::what()
	{
		return "coraxVM Exception";
	}




        corax_program_instruction::corax_program_instruction(instruction ins_, void* ptr_):
            ins(ins_),
            ptr(ptr_)
            {};
        corax_program_instruction::corax_program_instruction(instruction ins_,number_type val_):
            ins(ins_),
            val(val_)
        {};

        void corax_runtime::setOperator(operators::operators_interface* operators)
        {
            corax_runtime::_operators = operators;
        }

        void corax_runtime::setMemory(memory* mem)
        {
            corax_runtime::_mem = mem;
        }

        void corax_runtime::setFunction(math_func::function_interface* functions)
        {
            corax_runtime::_functions = functions;
        }

        number_type corax_runtime::run(interface::corax_program * prgm_)
        {

                CoraxVM::corax_program* prgm = dynamic_cast<CoraxVM::corax_program*>(prgm_);
                if(prgm == nullptr)
                {
                    throw CoraxVM::coraxOops("Bad program pointer");
                }

            std::stack<number_type> _stack;
             for(size_t i=0; i< prgm->instructions.size(); i++ )
             {
                 bool rpxu=false;
                 bool rxu=false;
                 corax_pointer_register* p_rpx = nullptr;
                 corax_register* p_rx=nullptr;
                  switch(prgm->instructions[i].ins & 0xF)
                  {
                  case instruction_flags::R1:
                    {
                        p_rx = &_r1;
                        rxu=true;
                    }
                    break;
                    case instruction_flags::R2:
                    {
                        p_rx = &_r2;
                        rxu=true;
                    }
                    break;
                    case instruction_flags::PR1:
                        {
                         p_rpx = &_pr1;
                         rpxu=true;
                        }
                        break;
                    case instruction_flags::PR2:
                        {
                            p_rpx = &_pr2;
                            rpxu=true;
                        }
                        break;
					case (instruction_flags::PR2 | instruction_flags::R2):
						{
							p_rpx = &_pr2;
							rpxu = true;
							p_rx = &_r2;
							rxu = true;
						}
						break;
					case instruction_flags::PR1 | instruction_flags::R1:
					{
						p_rx = &_r1;
						rxu = true;
						p_rpx = &_pr1;
						rpxu = true;
					}
						break;

                  }
                  
                 switch(prgm->instructions[i].ins & 0xF0 )
                 {
                 case instruction_set::CALL:
                    {
                        if(prgm->instructions[i].ins & instruction_flags::ARG2)
                        {
                             operators::operPtr ptr = (operators::operPtr)prgm->instructions[i].ptr;
                            if(_stack.size() < 2)
                            {
                                throw coraxOops("Stack don't contain necessary arguments for function call");
                            }
                            number_type tmp = _stack.top();
                            _stack.pop();
                            number_type tmp2 = _stack.top();
                            _stack.pop();
                            _stack.push(ptr(tmp2,tmp));

                        }
                        else if (prgm->instructions[i].ins & instruction_flags::ARG1)
                        {
							math_func::m_function::funcPtr ptr = (math_func::m_function::funcPtr)prgm->instructions[i].ptr;
                            number_type tmp = _stack.top();
                             if(_stack.size() < 1)
                            {
                                throw coraxOops("Stack don't contain necessary arguments for function call");
                            }
                            _stack.pop();
                            _stack.push(ptr(tmp));
                        }
                        else throw coraxOops("CALL called without register");
                    }
                    break;
                 case instruction_set::LD:
                    {
                       if(rxu && rpxu)
                       {
                            *p_rx = *(number_type*)(*p_rpx);
                       }
                       else if (rxu)
                       {
                           *p_rx= *(number_type*)prgm->instructions[i].ptr;
                       }
                       else throw coraxOops("LD called without register");
                    }
                    break;
                 case instruction_set::LDI:
                    {

                        if(rxu)
                        {
                            *p_rx =prgm->instructions[i].val;
                        }
                        else if(rpxu)
                        {
                            *p_rpx = prgm->instructions[i].ptr;
                        }
                        else throw coraxOops("LDI called without register");

                    }
                    break;
                    case instruction_set::SWITCH:
                    {
                        if(rpxu)
                        {
                            corax_pointer_register tmp =_pr1;
                            _pr1 = _pr2;
                            _pr2 = tmp;

                        }
                        else if(rxu)
                        {
                            corax_register tmp =corax_runtime::_r1;
                            _r1 = _r2;
                            _r2 = tmp;
                        }
                        else throw coraxOops("SWITCH was called without register");
                    }
                    break;
                 case instruction_set::POP:
                    {
                        if(rxu)
                        {
                            if(_stack.size() < 1)
                            {
                                throw coraxOops("Tried to pop from empty stack");
                            }

                            *p_rx = _stack.top();
                            _stack.pop();
                        }
                        else throw coraxOops("POP called without reigster");
                    }
                    break;
                 case instruction_set::PUSH:
                    {
                        if(rxu)
                        {
                            _stack.push(*p_rx);
                        }
                        else throw coraxOops("PUSH called without register");
                    }
                    break;
                case instruction_set::ST:
                    {
                        if(rpxu && rxu)
                        {
                                *(number_type*)(*p_rpx) = *p_rx;
                        }
                        else if(rxu)
                        {
                          *(number_type*)prgm->instructions[i].ptr = *p_rx;
                        }
                        else if (rpxu)
                        {
                            *(number_type*)(*p_rpx) = prgm->instructions[i].val;
                        }
                        else throw coraxOops("ST called without register");
                    }
                    break;
                default:
                    {
                        throw coraxOops("Unknown bytecode");
                    }
                    break;
                 }
             }
			 if (_stack.size() > 0)
			 {
				 return _stack.top();
			 }
			 else throw coraxOops("Stack empty on program termination");
           
        }

        corax_runtime::corax_runtime() :
            _operators(nullptr),
            _mem(nullptr),
            _functions(nullptr),
            _r1(0),
            _r2(0),
            _pr1(nullptr),
            _pr2(nullptr)
        {};

        corax_runtime::corax_runtime(operators::operators_interface* operators,memory* mem,math_func::function_interface* functions) :
            _operators(operators),
            _mem(mem),
            _functions(functions),
            _r1(0),
            _r2(0),
            _pr1(nullptr),
            _pr2(nullptr)
        {};
        void CoraxVM::corax_program::clear()
        {

            instructions.clear();
        }
        CoraxVM::corax_program::~corax_program()
        {

        }
        corax_runtime::~corax_runtime()
        {

        }






}

