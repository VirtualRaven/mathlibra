CC=g++

SCXXF = -Wall -fPIC -std=c++11 -I ./ -I ../common/ -I ../../core/CalcLib/ -fexceptions
SDXXF =  -g -fPIC -Wall -std=c++11 -I ./ -I ../common/ -I ../../core/CalcLib/ -fexceptions


LIBRARY= libcalcwrapper.so
STATIC_SHORT_LIB =calc
STATIC_LIB = lib$(STATIC_SHORT_LIB).a
BIN_DIR = ../../bin/
RAW_SOURCE = $(shell find ./*cpp  ../common/*.cpp)
SOURCE_FILES	:=$(filter-out ./dllmain.o, $(patsubst %.cpp,%.o,$(RAW_SOURCE)))


ifdef DEBUG
override SCXXF=$(SDXXF)
endif

VERSION_FILE = lib_git_version.h

default: git libary 


libary: CFLAGS=$(SCXXF) 
libary:   $(SOURCE_FILES) $(BIN_DIR)$(STATIC_LIB)
	-mkdir $(BIN_DIR)  		
	$(CC) -shared  $(CFLAGS) $? -L $(BIN_DIR) -l$(STATIC_SHORT_LIB) -ldl -o$(BIN_DIR)$(LIBRARY) 

git: 
	echo "const char *GIT_SHA_COMMIT = \"$(shell git rev-parse HEAD)\";" > $(VERSION_FILE)

.cpp.o: 
	$(CC) $(CFLAGS) -c  $*.cpp -o $*.o

clean:
	-rm $(BIN_DIR)$(LIBRARY)
	-rm $(SOURCE_FILES)
	-rm $(VERSION_FILE)
	

	
