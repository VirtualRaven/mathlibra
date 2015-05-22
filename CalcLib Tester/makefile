CC=g++

SCXXF = -Wall  -std=c++11 -I ./ -I ../"lib headers"/CalcLib -fexceptions
SDXXF =  -g  -Wall -std=c++11 -I ./ -I ../"lib headers"/CalcLib -fexceptions


EXECUTABLE=calc

RAW_SOURCE = $(shell find ./*cpp )
SOURCE_FILES	=$(patsubst %.cpp,%.o,$(RAW_SOURCE))
LIB_SHORT_NAME = calc
LIB_NAME = lib$(LIB_SHORT_NAME).a

ifdef DEBUG
override SCXXF=$(SDXXF)
endif

VERSION_FILE = lib_git_version.h

default: git $(LIB_NAME) client 


client: CFLAGS=$(SCXXF) 
client:   $(SOURCE_FILES)  	
	$(CC)   $(CFLAGS) $? -L . -l$(LIB_SHORT_NAME) -o$(EXECUTABLE).elf  

git: 
	echo "const char *GIT_SHA_COMMIT = \"$(shell git rev-parse HEAD)\";" > $(VERSION_FILE)

$(LIB_NAME): 
	cp ../../calculator-core/CalcLib/$(LIB_NAME) ./

.cpp.o: 
	@echo $*
	$(CC) $(CFLAGS) -c  $*.cpp -o $*.o

clean:
	-rm $(EXECUTABLE).elf
	-rm $(SOURCE_FILES)
	-rm $(VERSION_FILE)
	-rm $(LIB_NAME)
	
