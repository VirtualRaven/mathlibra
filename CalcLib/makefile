CC=g++

SCXXF = -Wall -fPIC -std=c++11 -I ./ -fexceptions
SDXXF =  -g -fPIC -Wall -std=c++11 -I ./ -fexceptions


LIBARY= libcalc

RAW_SOURCE = $(shell find ./*cpp */*.cpp)
SOURCE_FILES	=$(patsubst %.cpp,%.o,$(RAW_SOURCE))


ifdef DEBUG
override SCXXF=$(SDXXF)
endif

VERSION_FILE = core/lib_git_version.h

default: git  libary


libary: CFLAGS=$(SCXXF) 
libary:  $(CORE_FILES) $(SOURCE_FILES) $(INTERFACE_FILES) 		
	$(CC) -shared  $(CFLAGS) $? -o$(LIBARY).so 

git: 
	echo "const char *GIT_SHA_COMMIT = \"$(shell git rev-parse HEAD)\";" > $(VERSION_FILE)

.so: libary

.cpp.o: 
	@echo $*
	$(CC) $(CFLAGS) -c  $*.cpp -o $*.o

clean:
	-rm $(LIBARY).so
	-rm $(SOURCE_FILES)
	-rm $(VERSION_FILE)

	
