#include "core/token_list.h"
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
        if(!prev)
            return "{t: "+to_string(t) +",["+ std::to_string(start) +","+ std::to_string(end) +"],"+std::to_string(weight)+"}";
        else
            return "{t: "+to_string(t) +",["+ std::to_string(start) +","+ std::to_string(end) +"],"+std::to_string(weight)+",p="+ std::to_string(prev) +"}";
    }
}
