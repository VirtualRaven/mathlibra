#ifndef CORAX_VIRTUAL_MACHINE_H_INCLUDED
#define CORAX_VIRTUAL_MACHINE_H_INCLUDED
#include <cstdint>
#include <string>
#include <vector>
#include <stack>
#include "modules/functions.h"
#include "modules/memory.h"
#include "modules/operators.h"
#include "main.h"


//Virtual machine interpreting calc core libary byte codes
namespace CoraxVM
{
    struct coraxOops : public exception
    {
        coraxOops(std::string inf);
        const char* what();

    };

    typedef uint8_t byte;
    typedef byte instruction;
    typedef void*  corax_pointer_register;
    typedef number_type corax_register;
    //INSTRUCTION DEFINITION
    //The instruction is set in the hi nibble of the hi byte
    struct instruction_set
     {
        static const  instruction    LDI ; //Load constant value to register R1 or R2
        static const  instruction    LD  ; //Load variable value from address to R1 or R2
        static const  instruction    ST ;  //Store variable From R1 or R2 to address
        static const  instruction    PUSH; //Push r1 or r2 to stack
        static const  instruction    POP ; //Pop  from stack to r1 or r2
        static const  instruction    CALL;  //Call function from address, if
        static const  instruction    MOV;


    };
    //Bit flags which alter which registers are used
    struct instruction_flags
    {
        static const instruction R1;
        static const instruction R2;
        static const instruction PR1;
        static const instruction PR2;

        static const instruction ARG1;
        static const instruction ARG2;


    };


    struct corax_program_instruction
    {
        instruction ins;
        union
        {
            void* ptr;
            number_type val;
        };
        corax_program_instruction(instruction, void*);
        corax_program_instruction(instruction,number_type);
    };


    struct corax_program
    {
        std::vector<corax_program_instruction> instructions;
    };

    class corax
    {

        operators::operators_interface* _operators;
        memory* _mem;
        math_func::function_interface* _functions;

        corax_register _r1,_r2;
        corax_pointer_register _pr1,_pr2;


        public:

        void setOperator(operators::operators_interface* operators);
        void setMemory(memory* mem);
        void setFunction(math_func::function_interface* functions);
        number_type run(corax_program * prgm);
        number_type debug(corax_program *prgm);
        corax();
        corax(operators::operators_interface* operators,memory* mem,math_func::function_interface* functions);
        ~corax();
    };











}
#endif // CORAX_VIRTUAL_MACHINE_H_INCLUDED
