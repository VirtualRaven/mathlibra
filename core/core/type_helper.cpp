/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
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
