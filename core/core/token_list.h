#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H
#include <cstddef>
#include <vector>
#include <string>
namespace token_list{

    enum struct type{
            PAR ,
            MAT ,
            OPER,
            FUNC,
            STR ,
            VAL ,
            VAR
    };

    class item {
        const type t;
        const size_t start;
        const size_t end;
        const size_t prev;
        const bool is_closing;
        const size_t weight;
    public:
        item(const type t,const size_t w,const size_t i,const size_t j) ;
        item(const type t,const size_t w,const size_t i, const size_t j, const size_t prev);
        operator std::string() const;
    };
    typedef std::vector<item> token_list;
}
#endif
