#include "core/type_helper.h"
#include "type.h"

type* make_type(double x)
{
    return new num_mat(&x,1,1);
}

type* make_type(std::string x)
{
        return new char_mat(x.c_str(),1,x.size());
}
