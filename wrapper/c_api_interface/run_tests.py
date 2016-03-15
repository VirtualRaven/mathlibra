# Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/. 
import mathlibra
import genTests
import sys

file_name= "test_report.txt"
number_of_tests=1000
class testException:
    def __init__(self,mathlibra_error,test_expr,test_number):
        self.test_msg = test_expr
        self.test = test_number
        self.math_msg = mathlibra_error

    def testExpr(self):
        return self.test_msg
    def mathError(self):
        return self.math_msg
    def testNumber(self):
        return str(self.test)

    def __str__(self):
        return self.math_msg + "\n" +self.test_msg 

f = open(file_name,"w")
exit_str = "Tests failed, see " + file_name +" for more info."


try:
    x = mathlibra.mathlibra()
    tests = genTests.createTests(number_of_tests)
    varExprs=genTests.createMathlibraVars(tests["env"])
    print >>f, "Defined variables:"
    for expr in varExprs:
        print >>f,expr
        x.interpret_arg(expr)
        x.execute_arg()
    print>>f,""
    i=1
    print "Running "+ str(number_of_tests) + " tests..."
    for test in tests['tests']:
            try:
                x.interpret_arg(test[0])
                result = x.execute_arg()
                if result != test[1]:
                    print >>f,"Test " + str(i) +" failed.\n"
                    print >>f,"Exception:\nMathlibra returned "+str(result)+" but expected value was "+str(test[1]) 
                    print >>f,"\nExpression:"
                    print >>f,test[0]
                    print exit_str
                    sys.exit(1)

            except mathlibra.mathlibra_exception as e:
                raise testException(str(e),test[0],i)
    print "Tests successfull"
    sys.exit(0)


except mathlibra.mathlibra_exception as e:
    print >>f,"Tests failed due to library exception.\n\nException:\n"
    print >>f,e
    print exit_str  
    sys.exit(1)

except testException as e:
    print >>f,"Test " + e.testNumber() + " failed."
    print >>f,"\nException:"
    print >>f,e.mathError()
    print >>f,"\nExpression:"
    print >>f,e.testExpr()
    print exit_str  
    sys.exit(1)
