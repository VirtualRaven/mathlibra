import ctypes

class mathlibra_exception(Exception):
    def __init__(self,etype,message,crit,id_num):
        self.info = message
        self.etype=etype
        self.critical = crit
        self.id_num = id_num
    def __str__(self):
        return self.etype + " " + self.info + " (" +str(self.id_num) + ")"

class func_obj_c (ctypes.Structure):
    _fields_ = [
                ("name",ctypes.c_char_p),
                ("tag",ctypes.c_char_p),
                ("doc",ctypes.c_char_p),
                ("disp_name",ctypes.c_char_p)]
class mem_obj_c (ctypes.Structure):
    _fields_ = [
        ("name",ctypes.c_char_p),
        ("val",ctypes.c_double),
        ("const",ctypes.c_bool)
        ]
class error_obj_c(ctypes.Structure):
    _fields_ = [
                ("type",ctypes.c_char_p),
                ("description",ctypes.c_char_p),
                ("critical",ctypes.c_bool),
                ("id_number",ctypes.c_uint)]
class func_obj_array_c (ctypes.Structure):
    _fields_ = [("array",ctypes.POINTER(func_obj_c)),("size",ctypes.c_uint)]

class mathlibra:
    def __init__(self):
        #load dll
        self.lib_instance = ctypes.CDLL("./libmathlibra_C.so")
        
        #Set up the instance creation function
        self.handle =ctypes.c_void_p()
        self.__handle_create = self.lib_instance.create_handle
        self.__handle_create.restype = ctypes.c_void_p 
        
        #Accuire an instance
        self.handle = self.__handle_create()
        
        
        #Error managment functions
        #Type is set to void pointer to ensure that ctypes to not convert it to an python string
        self.__get_excep=self.lib_instance.mathlibra_error_info
        self.__get_excep.restype =error_obj_c

        
        #create internal functions for library exported functions
        self.__interpret_arg = self.lib_instance.interpret_arg
        
        self.__free_func_obj_array_c =self.lib_instance.free_func_obj_array

        self.__execute_arg = self.lib_instance.execute_arg
        self.__execute_arg.restype = ctypes.c_void_p
        
        self.__error = self.lib_instance.mathlibra_error
        self.__error.restype = ctypes.c_bool
        
        self.__func_get=self.lib_instance.func_get
        self.__func_get.restype=func_obj_array_c
        
        self.__free = self.lib_instance.free_handle
        self.__enable_plugins = self.lib_instance.enable_plugins
        
        self.__mem_size = self.lib_instance.mem_size
        self.__mem_size.restype =ctypes.c_uint

        self.__mem_get_index=self.lib_instance.mem_get_index
        self.__mem_get_index.restype = mem_obj_c
        
        self.__mem_get = self.lib_instance.mem_get
        self.__mem_get.restype=mem_obj_c

        self.__mem_set= self.lib_instance.mem_set

        self.__free_mem_obj=self.lib_instance.free_mem_obj
        self.__is_number = self.lib_instance.isNumber
        self.__is_number.restype=ctypes.c_bool
        self.__to_number = self.lib_instance.toNumber
        self.__to_number.restype=ctypes.c_double 


    #Function to convert c struct to python data types
    def __from_func_obj_array_c(self,obj):
        tmp_list = []
        for index in range(obj.size):
            tmp_list.append(self.__from_func_obj_c(obj.array[index]))
        
        self.__free_func_obj_array_c(obj)
        return tmp_list

    def __from_func_obj_c(self,obj):
        return { 'name':obj.name, 'doc':obj.doc, 'tag':obj.tag,'disp_name':obj.disp_name}

    def __del__(self):
        self.__free(self.handle)

    def __check_error(self):
        if self.__error(self.handle):
            msg=self.__get_excep(self.handle)
            raise mathlibra_exception(msg.type,msg.description,msg.critical,msg.id_number)
    
    def interpret_arg(self,expression):
        self.__interpret_arg(self.handle,ctypes.c_char_p(expression))
        self.__check_error()
    
    def get_funcs(self):
        val = self.__from_func_obj_array_c(self.__func_get(self.handle))
        self.__check_error()
        return val
    def get_mem(self):
        num = self.__mem_size(self.handle)
        tmp_list = []
        for index in range(num):
            c =self.__mem_get_index(self.handle,index)
            tmp_list.append({'name':c.name, 'value':c.val, 'const':c.const})
        return tmp_list

    def execute_arg(self):
        val = self.__execute_arg(self.handle)
        self.__check_error()
        if not self.__is_number(val):
            raise mathlibra_exception("PYTHON_WRAPPER","Mathlibra returned non double value which is not supported by this version of the python wrapper",true,-1)

        return self.__to_number(val)
    
    def enable_plugins(self):
        self.__enable_plugins(self.handle)
        self.__check_error

