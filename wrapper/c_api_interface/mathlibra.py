import ctypes

class mathlibra_exception(Exception):
    __init__(self,message):
        self.info = message
    def __str__(self):
        return self.info

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
        
        #create internal functions for library exported functions
        self.__interpret_arg = self.lib_instance.interpret_arg
        
        self.__execute_arg = self.lib_instance.execute_arg
        self.__execute_arg.restype = ctypes.c_double
        
        self.__error = self.lib_instance.mathlibra_error
        self.__error.restype = ctypes.c_bool
        
        self.__free = self.lib_instance.free_handle
        self.__enable_plugins = self.lib_instance.enablei_plugins()
    
    def __del__(self):
        self.__free(self.handle)

    def check_error()
        if self.__error(self.handle):
            raise mathlibra_exception("failed")
    
    def interpret_arg(self,expression):
        self.__interpret_arg(self.handle,ctypes.c_char_p(expression))
        self.check_error()
    
    def execute_arg(self):
        val = self.__execute_arg(self.handle)
        self.check_error()
        return val
    
    def enablePlugins():
        self.__enable_plugins()
        self.check_error

