CC=g++

SCXXF = -Wall  -std=c++11 -I ./ -I ../"lib headers"/CalcLib -fexceptions
SDXXF =  -g  -Wall -std=c++11 -I ./ -I ../"lib headers"/CalcLib -fexceptions


EXECUTABLE=calc

RAW_SOURCE = $(shell find ./*cpp */*.cpp)
SOURCE_FILES	=$(patsubst %.cpp,%.o,$(RAW_SOURCE))
LIB_NAME = libcalc.so

ifdef DEBUG
override SCXXF=$(SDXXF)
endif

VERSION_FILE = lib_git_version.h

default: git  client


client: CFLAGS=$(SCXXF) 
client:  $(CORE_FILES) $(SOURCE_FILES) $(INTERFACE_FILES) 		
	$(CC)   $(CFLAGS) $? -o$(EXECUTABLE).elf -l$(LIB_NAME) 

git: 
	echo "const char *GIT_SHA_COMMIT = \"$(shell git rev-parse HEAD)\";" > $(VERSION_FILE)

$(LIB_NAME).so: $(LIB_NAME)
	cp ../../calculator-core/CalcLib/$(LIB_NAME) ./


.cpp.o: 
	@echo $*
	$(CC) $(CFLAGS) -c  $*.cpp -o $*.o

clean:
	-rm $(EXECUTABLE).elf
	-rm $(SOURCE_FILES)
	-rm $(VERSION_FILE)
	-rm $(LIB_NAME)
	
