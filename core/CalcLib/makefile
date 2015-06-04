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
libary:  $(SOURCE_FILES)  		
	ar -cvq $(LIBARY).a   $?  

git: 
	echo "const char *GIT_SHA_COMMIT = \"$(shell git rev-parse HEAD)\";" > $(VERSION_FILE)



.cpp.o: 
	@echo $*
	$(CC) $(CFLAGS) -DLINUX=true -c  $*.cpp  -o $*.o


clean:
	-rm $(LIBARY).a
	-rm $(SOURCE_FILES)
	-rm $(VERSION_FILE)

	
