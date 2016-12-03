#include "core/token_list.h"
#include <sstream>
namespace token_list{
    
    inline std::string to_string(const type t){
        switch(t)
        {
            case type::PAR    :   return "paranthese";
            case type::MAT    :   return "matrix";
            case type::OPER   :   return "operator";
            case type::FUNC   :   return "function";
            case type::STR    :   return "string";
            case type::VAL    :   return "value" ;
            case type::VAR    :   return "variable";
            default: return "";
        }

    }


    item::item(const type t,const size_t w,const size_t i,const size_t j) : 
        t(t),start(i),end(j),prev(0),is_closing(false),weight(w) {}

    item::item(const type t,const size_t w,const size_t i, const size_t j, const size_t prev):
        t(t),start(i),end(j),prev(prev),is_closing(true),weight(w) {}
    
    item::operator std::string() const {
        std::stringstream s;
        if(!prev)
            s << "{t: "<<to_string(t) <<",["<< start <<","<< end <<"],"<<weight<<"}";
        else
            s << "{t: "<<to_string(t) <<",["<< start <<","<< end <<"],"<<weight<<",p="<< prev << "}";
        return s.str();
    }
}
