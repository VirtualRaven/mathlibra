import ctypes

class mathlibra_exception(Exception):
    def __init__(self,message):
        self.info = message
    def __str__(self):
        return self.info

class func_obj:
    def __init__ (self,_name,_tag,_doc,_disp_name):
        self.name = _name
        self.tag = _tag
        self.doc = _doc
        self.disp_name = _disp_name

class func_obj_c (ctypes.Structure):
    _fields_ = [
                ("name",ctypes.c_char_p),
                ("tag",ctypes.c_char_p),
                ("doc",ctypes.c_char_p),
                ("disp_name",ctypes.c_char_p)]

class func_obj_array_c (ctypes.Structure):
    _fields_ = [("array",ctypes.POINTER(func_obj_c)),("size",ctypes.c_uint)]

class mathlibra:
    def __init__(self):
        #load dll
        self.lib_instance = ctypes.CDLL("./libmathlibra_C.so")
        
        #Set up the instance creator function
        self.handle =ctypes.c_void_p()
        self.handle_creator = self.lib_instance.create_handle
        self.handle_creator.restype = ctypes.c_void_p 
        
        #Accurie an instance
        self.handle = self.handle_creator()
        
        self.__free_error_info = self.lib_instance.free_error_info
        self.__free_error_info.restype = None
        self.__free_error_info.argtypes=[ctypes.c_char_p]
        #create internal functions for library exported functions
        self.__interpret_arg = self.lib_instance.interpret_arg
        
        self.__free_func_obj_array_c =self.lib_instance.free_func_obj_array

        self.__execute_arg = self.lib_instance.execute_arg
        self.__execute_arg.restype = ctypes.c_double
        
        self.__error = self.lib_instance.mathlibra_error
        self.__error.restype = ctypes.c_bool
        
        self.__func_get=self.lib_instance.func_get
        self.__func_get.restype=func_obj_array_c
        
        self.__get_excep=self.lib_instance.mathlibra_error_info
        self.__get_excep.restype =ctypes.c_char_p 

        self.__free = self.lib_instance.free_handle
        self.__enable_plugins = self.lib_instance.enable_plugins
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

    def check_error(self):
        if self.__error(self.handle):
            msg=self.__get_excep(self.handle)
            py_msg = msg.value
            self.__free_error_info(msg)
            #raise mathlibra_exception(msg)
    
    def interpret_arg(self,expression):
        self.__interpret_arg(self.handle,ctypes.c_char_p(expression))
        self.check_error()
    
    def get_funcs(self):
        val = self.__from_func_obj_array_c(self.__func_get(self.handle))
        self.check_error()
        return val

    def execute_arg(self):
        val = self.__execute_arg(self.handle)
        self.check_error()
        return val
    
    def enablePlugins(self):
        self.__enable_plugins(self.handle)
        self.check_error

