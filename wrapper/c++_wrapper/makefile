CC=g++

SCXXF = -Wall -fPIC -std=c++11 -I ./ -I ../ -I ../"lib headers"/CalcLib/ -fexceptions
SDXXF =  -g -fPIC -Wall -std=c++11 -I ./ -I ../ -I ../"lib headers"/CalcLib/ -fexceptions


LIBARY= libcalcwrapper
STATIC_SHORT_LIB =calc
STATIC_LIB = lib$(STATIC_SHORT_LIB).a
RAW_SOURCE = $(shell find ./*cpp  ../*.cpp)
SOURCE_FILES	:=$(filter-out ./dllmain.o, $(patsubst %.cpp,%.o,$(RAW_SOURCE)))


ifdef DEBUG
override SCXXF=$(SDXXF)
endif

VERSION_FILE = lib_git_version.h

default: git $(STATIC_LIB) libary 


libary: CFLAGS=$(SCXXF) 
libary:   $(SOURCE_FILES)  		
	$(CC) -shared  $(CFLAGS) $? -L . -l$(STATIC_SHORT_LIB)  -o$(LIBARY).so 

git: 
	echo "const char *GIT_SHA_COMMIT = \"$(shell git rev-parse HEAD)\";" > $(VERSION_FILE)

$(STATIC_LIB):
	cp ../../calculator-core/CalcLib/$(STATIC_LIB) ./

.cpp.o: 
	$(CC) $(CFLAGS) -c  $*.cpp -o $*.o

clean:
	-rm $(LIBARY).so
	-rm $(SOURCE_FILES)
	-rm $(VERSION_FILE)
	-rm $(STATIC_LIB)

	
