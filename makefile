CC=g++

CXXF=  -Wall -std=c++11 -I ./headers/ -I ./headers/client/ -I ./headers/core/ -fexceptions
DXXF= -g  -Wall -std=c++11 -I ./headers/ -I ./headers/client/ -I ./headers/core/ -fexceptions
SCXXF = -Wall -std=c++11 -I ./headers/ -I ./headers/client/ -I ./headers/core/ -fexceptions
SDXXF =  -g -Wall -std=c++11 -I ./headers/ -I ./headers/client/ -I ./headers/core/ -fexceptions

CURRENT_F = ""

ifdef DEBUG
override CFLAGS=$(DFLAGS)
endif

SOURCE_DIR =  ./source/
CLIENT_DIR = client/
MODULE_DIR = modules/
CORE_DIR = core/
INTERFACE_DIR = interface/

LIBARY= mathinterpreter
EXECUTABLE=  calc

default:  client


client: $(patsubst %.cpp,%.o,$(wildcard $(SOURCE_DIR)$(CLIENT_DIR)*.cpp)) $(patsubst %.cpp,%.o,$(wildcard $(SOURCE_DIR)*.cpp)) libary
	$(CC) $(CFLAGS) $? -o$(EXECUTABLE) 

libary:  $(patsubst %.cpp,%.o,$(wildcard $(SOURCE_DIR)$(CORE_DIR)*.cpp)) $(patsubst %.cpp,%.o,$(wildcard $(SOURCE_DIR)*.cpp)) 
	
	$(CC) -shared -fPIC $(CFLAGS) $? -o$(LIBARY).so 


.cpp.o: 
	@echo $*
	$(CC) $(CFLAGS) -c  $*.cpp -o $*.o


clean:
	-rm $(patsubst %.cpp,%.o,$(wildcard $(ODIR)*.cpp)) 
	-rm $(EXECUTABLE) $(EXECUTABLE)_debug
	
