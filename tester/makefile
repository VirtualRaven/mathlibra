CC=g++

SCXXF = -Wall  -std=c++11 -I ./ -I ../core/ -fexceptions
SDXXF =  -g  -Wall -std=c++11 -I ./ -I ../core/ -fexceptions


EXECUTABLE=calc

BIN_DIR = ../bin/
RAW_SOURCE = $(shell find ./*cpp )
SOURCE_FILES	=$(patsubst %.cpp,%.o,$(RAW_SOURCE))
LIB_SHORT_NAME = calc
LIB_NAME = lib$(LIB_SHORT_NAME).a

ifdef DEBUG
override SCXXF=$(SDXXF)
endif

VERSION_FILE = lib_git_version.h

default: git client $(BIN_DIR)$(LIB_NAME)


client: CFLAGS=$(SCXXF) 
client:   $(SOURCE_FILES) 
	-mkdir $(BIN_DIR)  	
	$(CC)   $(CFLAGS) $?  -L $(BIN_DIR) -l$(LIB_SHORT_NAME) -ldl -o$(BIN_DIR)$(EXECUTABLE).elf  

git: 
	echo "const char *GIT_SHA_COMMIT = \"$(shell git rev-parse HEAD)\";" > $(VERSION_FILE)



.cpp.o: 
	@echo $*
	$(CC) $(CFLAGS) -c  $*.cpp -o $*.o

clean:
	-rm $(BIN_DIR)$(EXECUTABLE).elf
	-rm $(SOURCE_FILES)
	-rm $(VERSION_FILE)

	
