#ifndef CORAX_VIRTUAL_MACHINE_H_INCLUDED
#define CORAX_VIRTUAL_MACHINE_H_INCLUDED
#include <cstdint>
#include <string>
#include <vector>
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

    typedef uint16_t instruction;
    typedef uint8_t byte;

    //INSTRUCTION DEFINITION
    //The instruction is set in the hi nibble of the hi byte
    struct instruction_set
     {
        static const  instruction    LDI ;
        static const  instruction    LD  ;
        static const  instruction    ST ;
        static const  instruction    PUSH;
        static const  instruction    POP ;
        static const  instruction    CALL;


    };
    //Bit flags which can alter the behavior of each instruction
    struct instruction_flags
    {
        static const instruction FLAG1;
        static const instruction FLAG2;
        static const instruction FLAG3;
        static const instruction FLAG4;

    };



    struct symbol
    {
        byte value;
        std::string name;
        enum type
        {
            VARIABLE,
            FUNCTIONARG1,
            OPERATOR,
            FUNC_PTR //Special reserved value only to be used  in searches for FUNCTIONS or OPERATORS
        };
        type symbol_type;
        symbol(byte,std::string,type);
    };

    class symbol_table
    {
        std::vector<symbol> _funcs;
        std::vector<symbol> _variables;
        short _find(byte,symbol::type);
        short _find(std::string name, symbol::type symbol_type);
    public:
        byte add(std::string name, symbol::type symbol_type);
        std::string get(byte num,symbol::type symbol_type = symbol::type::FUNC_PTR);

    };

    struct corax_program
    {

    };

    class corax
    {

        operators::operators_interface* _operators;
        memory* _mem;
        math_func::function_interface* _functions;
        public:

        void setOperator(operators::operators_interface* operators);
        void setMemory(memory* mem);
        void setFunction(math_func::function_interface* functions);
        number_type run(corax_program * prgm);
        corax();
        corax(operators::operators_interface* operators,memory* mem,math_func::function_interface* functions);
        ~corax();
    };

}
#endif // CORAX_VIRTUAL_MACHINE_H_INCLUDED
