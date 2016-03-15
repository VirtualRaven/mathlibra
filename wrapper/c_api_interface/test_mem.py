# Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/. 
import mathlibra
import genTests
import sys
try:
    x = mathlibra.mathlibra()
    print "Pre assigment"
    print x.get_mem()
    x.interpret_arg("x=2")
    x.execute_arg()
    x.interpret_arg("y=3")
    x.execute_arg()
    print "Post assigment"
    print x.get_mem()
    print "Loaded functions"
    print x.get_funcs()

except mathlibra.mathlibra_exception as e:
    print >>f,"Tests failed due to library exception.\n\nException:\n"
    print >>f,e
    print exit_str  
    sys.exit(1)


