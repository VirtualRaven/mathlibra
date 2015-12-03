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


