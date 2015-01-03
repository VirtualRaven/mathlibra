CC=g++

CXXF=  -Wall -std=c++11 -I ./headers/ -I ./headers/client/ -I ./headers/core/ -fexceptions
DXXF= -g  -Wall -std=c++11 -I ./headers/ -I ./headers/client/ -I ./headers/core/ -fexceptions
SCXXF = -Wall -fPIC -std=c++11 -I ./headers/ -I ./headers/client/ -I ./headers/core/ -fexceptions
SDXXF =  -g -fPIC -Wall -std=c++11 -I ./headers/ -I ./headers/client/ -I ./headers/core/ -fexceptions


SOURCE_DIR =  ./source/
CLIENT_DIR = client/
MODULE_DIR = modules/
CORE_DIR = core/
INTERFACE_DIR = interface/

LIBARY= mathinterpreter
EXECUTABLE=  calc
SOURCE_FILES	=$(patsubst %.cpp,%.o,$(wildcard $(SOURCE_DIR)*.cpp))
CLIENT_FILES	=$(patsubst %.cpp,%.o,$(wildcard $(SOURCE_DIR)$(CLIENT_DIR)*.cpp))
MODULE_FILES	=$(patsubst %.cpp,%.o,$(wildcard $(SOURCE_DIR)$(MODULE_DIR)*.cpp))
CORE_FILES	=$(patsubst %.cpp,%.o,$(wildcard $(SOURCE_DIR)$(CORE_DIR)*.cpp)) 
INTERFACE_FILES	=$(patsubst %.cpp,%.o,$(wildcard $(SOURCE_DIR)$(INTERFACE_DIR)*.cpp)) 

ifdef DEBUG
override CXXF=$(DXXF)
override SCXXF=$(SDXXF)
endif
VERSION_FILE = source/gitversion.cpp
SOURCE_FILES +=$(patsubst %.cpp,%.o,$(VERSION_FILE))


default:  client


client: CFLAGS= $(CXXF)
client: $(CLIENT_FILES) $(SOURCE_FILES)  $(MODULE_FILES) $(LIBARY).so
	$(CC) $(CFLAGS) $? -o$(EXECUTABLE) 

libary: CFLAGS=$(SCXXF)
libary: $(CORE_FILES) $(SOURCE_FILES) $(INTERFACE_FILES) 		
	$(CC) -shared  $(CFLAGS) $? -o$(LIBARY).so 

$(VERSION_FILE): .git/HEAD .git/index
	echo "const char *git_version = \"$(shell git rev-parse HEAD)\";" > $@

.so: libary

.cpp.o: 
	@echo $*
	$(CC) $(CFLAGS) -c  $*.cpp -o $*.o

clean:
	-rm $(LIBARY).so
	-rm $(EXECUTABLE)
	-rm $(SOURCE_FILES)
	-rm $(CLIENT_FILES)
	-rm $(MODULE_FILES)
	-rm $(CORE_FILES)
	-rm $(INTERFACE_FILES)
	
