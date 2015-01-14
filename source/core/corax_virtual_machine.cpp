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


             std::stack<number_type> _stack;
             for(size_t i=0; i< prgm->instructions.size(); i++ )
             {
                 bool rpxu=false;
                 bool rxu=false;
                 corax_pointer_register* p_rpx;
                 corax_register* p_rx;
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
                  }
                  corax_pointer_register& rpx= *p_rpx;
                 corax_register& rx= *p_rx;
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
                            math_func::function::funcPtr ptr = (math_func::function::funcPtr)prgm->instructions[i].ptr;
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
                            rx = *(number_type*)rpx;
                       }
                       else if (rxu)
                       {
                           rx= *(number_type*)prgm->instructions[i].ptr;
                       }
                       else throw coraxOops("LD called without register");
                    }
                    break;
                 case instruction_set::LDI:
                    {

                        if(rxu)
                        {
                            rx =prgm->instructions[i].val;
                        }
                        else if(rpxu)
                        {
                            rpx = prgm->instructions[i].ptr;
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
                        else if(rpx)
                        {
                            corax_register tmp =corax::_r1;
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

                            rx = _stack.top();
                            _stack.pop();
                        }
                        else throw coraxOops("POP called without reigster");
                    }
                    break;
                 case instruction_set::PUSH:
                    {
                        if(rxu)
                        {
                            _stack.push(rx);
                        }
                        else throw coraxOops("PUSH called without register");
                    }
                    break;
                case instruction_set::ST:
                    {
                        if(rpxu && rxu)
                        {
                                *(number_type*)rpx = rx;
                        }
                        else if(rxu)
                        {
                          *(number_type*)prgm->instructions[i].ptr = rx;
                        }
                        else if (rpxu)
                        {
                            *(number_type*)rpx = prgm->instructions[i].val;
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
            return _stack.top();
        }

        corax::corax() :
            _operators(nullptr),
            _mem(nullptr),
            _functions(nullptr),
            _r1(0),
            _r2(0),
            _pr1(nullptr),
            _pr2(nullptr)
        {};

        corax::corax(operators::operators_interface* operators,memory* mem,math_func::function_interface* functions) :
            _operators(operators),
            _mem(mem),
            _functions(functions),
            _r1(0),
            _r2(0),
            _pr1(nullptr),
            _pr2(nullptr)
        {};

        corax::~corax()
        {

        }

        number_type corax::debug(corax_program *prgm)
        {
            std::stack<number_type> _stack;
            for(size_t i=0; i< prgm->instructions.size(); i++ )
             {
                 bool rpxu=false;
                 bool rxu=false;
                 corax_pointer_register* p_rpx;
                 corax_register* p_rx;
                  switch(prgm->instructions[i].ins & 0xF)
                  {
                  case instruction_flags::R1:
                    {
                        p_rx = &_r1;
                        std::cout << "PC: "<< i << " Selected register R1\n";
                        rxu=true;
                    }
                    break;
                    case instruction_flags::R2:
                    {
                        p_rx = &_r2;
                        std::cout << "PC: "<< i << " Selected register R2\n";
                        rxu=true;
                    }
                    break;
                    case instruction_flags::PR1:
                        {
                         p_rpx = &_pr1;
                         std::cout << "PC: "<< i << " Selected register PR1\n";
                         rpxu=true;
                        }
                        break;
                    case instruction_flags::PR2:
                        {
                            p_rpx = &_pr2;
                            std::cout << "PC: "<< i << " Selected register PR2\n";
                            rpxu=true;
                        }
                        break;
                  }
                  corax_pointer_register& rpx= *p_rpx;
                 corax_register& rx= *p_rx;
                 switch(prgm->instructions[i].ins & 0xF0 )
                 {
                 case instruction_set::CALL:
                    {
                        if(prgm->instructions[i].ins & instruction_flags::ARG2)
                        {
                             std::cout << "PC: "<< i << " CALL, ARG2\n";
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
                            std::cout << "PC: "<< i << " CALL ARG1\n";
                          math_func::function::funcPtr ptr = (math_func::function::funcPtr)prgm->instructions[i].ptr;
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
                       if(rxu)
                       { std::cout << "PC: "<< i << " LD\n";
                            rx = *(number_type*)prgm->instructions[i].ptr;
                       }
                       else throw coraxOops("LD called without register");
                    }
                    break;
                 case instruction_set::LDI:
                    {

                        if(rxu)
                        {
                            std::cout << "PC: "<< i << " LDI\n";
                            rx =prgm->instructions[i].val;
                        }
                        else throw coraxOops("LDI called without register");

                    }
                    break;
                 case instruction_set::SWITCH:
                    {
                        if(rpxu)
                        {
                            corax_pointer_register tmp =corax::_pr1;
                            _pr1 = _pr2;
                            _pr2 = tmp;

                        }
                        else if(rpx)
                        {
                            corax_register tmp=corax::_r1;
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
                            std::cout << "PC: "<< i << " POP\n";
                            if(_stack.size() < 1)
                            {
                                throw coraxOops("Tried to pop from empty stack");
                            }
                            rx = _stack.top();
                            _stack.pop();
                        }
                        else throw coraxOops("POP called without reigster");
                    }
                    break;
                 case instruction_set::PUSH:
                    {
                        if(rxu)
                        {
                            std::cout << "PC: "<< i << " PUSH\n";
                            _stack.push(rx);
                        }
                        else throw coraxOops("PUSH called without register");
                    }
                    break;
                case instruction_set::ST:
                    {
                        if(rpxu && rxu)
                        {
                            std::cout << "PC: "<< i << " ST\n";
                                *(number_type*)rpx = rx;
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
            return _stack.top();
        }





}

