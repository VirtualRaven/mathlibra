CC=g++

SCXXF = -Wall -fPIC -std=c++11 -I ./ -I ../ -I ../common/ -I ../../core/CalcLib/ -fexceptions
SDXXF =  -g -fPIC -Wall -std=c++11 -I ./ -I ../ -I ../common/ -I ../../core/CalcLib/ -fexceptions

EXECUTABLE=test.elf
LIBARY= libcalcwrapper

SHARED_SHORT_LIB =calcwrapper
SHARED_LIB = lib$(SHARED_SHORT_LIB).so
BIN_DIR = ../../bin/
RAW_SOURCE = $(shell find ./*cpp  )
SOURCE_FILES	:=$(filter-out ./dllmain.o, $(patsubst %.cpp,%.o,$(RAW_SOURCE)))


ifdef DEBUG
override SCXXF=$(SDXXF)
endif

VERSION_FILE = lib_git_version.h

default: git libary $(BIN_DIR)$(SHARED_LIB)


libary: CFLAGS=$(SCXXF) 
libary:   $(SOURCE_FILES) 
	-mkdir $(BIN_DIR)  		
	$(CC)   $(CFLAGS) $?  -Wl,-rpath=. -L$(BIN_DIR) -l$(SHARED_SHORT_LIB)  -o$(BIN_DIR)$(EXECUTABLE)

git: 
	echo "const char *GIT_SHA_COMMIT = \"$(shell git rev-parse HEAD)\";" > $(VERSION_FILE)

.cpp.o: 
	$(CC) $(CFLAGS) -c  $*.cpp -o $*.o

clean:
	-rm $(BIN_DIR)$(EXECUTABLE)
	-rm $(SOURCE_FILES)
	-rm $(VERSION_FILE)
	

	
