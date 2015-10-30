import random
from mathlibra import mathlibra
import math

class node_data:
    
    def sub1(self):
        return self.branch_1
    def sub2(self):
        return self.branch_2
    
    def build(self,b1,b2):
        self.branch_2 = b2
        self.branch_1 = b1

    def show(self):
        return "NOT_IMPLEMETED"
    def eval(self,enviroment):
        raise "EVAL_NOT_IMPLEMETED"

class val (node_data):

    def __init__(self,value):
        self.value = value
    
    def eval(self,enviroment):
        return self.value
    
    def show(self):
        return str(self.value)

class var(node_data):
    
    def __init__(self,var_name):
        self.name=var_name
    
    def eval(self,enviroment):
        if self.name in enviroment:
            return enviroment[self.name]
        else:
            raise("Requested variable not found")

    def show(self):
        return self.name

class  opr(node_data):
    def __init__(self,opr_char,oper_func):
            self.cstr=opr_char
            self.f = oper_func

    def eval(self,enviroment):
        return self.f( self.sub1().eval(enviroment) , self.sub2().eval(enviroment))

    def show(self):
        return "(" + self.sub1().show() +")" + self.cstr + "("+ self.sub2().show() + ")" 

class  func(node_data):
    def __init__(self,func_str,func):
            self.fstr=func_str
            self.f = func

    def eval(self,enviroment):
        return self.f( self.sub1().eval(enviroment))

    def show(self):
        return self.fstr + "(" + self.sub1().show() +")"

def oneOf(list):
    index = random.randrange(0,len(list))
    return list[index]

def genTree(prob,func_list,var_list,opr_list):
    rand = random.randrange(1,prob)
    node = node_data() 
    
    if rand == 1:
        node = val(random.randrange(1,1000))
    elif rand ==2:
        node=var(oneOf(var_list))
    else:
        if rand  < prob //2:
            func_data = oneOf(func_list)
            node=func(func_data[0],func_data[1])
        else:
            opr_data = oneOf(opr_list)
            node=opr(opr_data[0],opr_data[1])
        
        node.build(
                    genTree(prob//2,func_list,var_list,opr_list),
                    genTree(prob//2,func_list,var_list,opr_list)
                    )

    return node    

def mkEnviroment(vars):
    tmp = {}
    for var in vars:
        tmp[var]=random.randrange(1,1000)
    return tmp

def mkTestTupel(testConds):
    expr = genTree(100,testConds["funcs"],testConds["vars"],testConds["oprs"])
    res =expr.eval(testConds["env"])
    return (expr.show(),res)

def mkTestConditions():
    testConds = {}
    testConds["funcs"] =[("floor",lambda x:math.floor(x))]
    testConds["oprs"]  =[("-",lambda x,y:x-y),("+",lambda x,y:x+y)]
    testConds["vars"] = ["X","Y","Z"]
    testConds["env"] = mkEnviroment(testConds["vars"])
    return testConds

def createTests(num):
    conds=mkTestConditions()
    test={}
    test["env"] = conds["env"]
    test["tests"] = []
    for index in range(0,num):
        test["tests"].append(mkTestTupel(conds))
    return test

def createMathlibraVars(env):
    expr = []
    for e in env.keys():
        expr.append(e+"="+str(env[e]))
    return expr

def printTests(test):
    print "#######################\n    Enviroment:\n"
    print test["env"] 
    print "\n######################\n"
    i=1
    for t in test["tests"]:
        print str(i)+": ", t 
        i=i+1


