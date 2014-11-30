CC=g++
CFLAGS=  -Wall -std=c++11 -I ./headers/ -fexceptions
DFLAGS= -g  -Wall -std=c++11 -I ./headers/ -fexceptions
ifdef DEBUG
override CFLAGS=$(DFLAGS)
endif
ODIR =  ./source/
EXECUTABLE=  calc

default:  $(patsubst %.cpp,%.o,$(wildcard $(ODIR)*.cpp))
	$(CC) $(CFLAGS)  $?  -o $(EXECUTABLE)


.cpp.o: 
	@echo $*
	$(CC) $(CFLAGS) -c  $*.cpp -o $*.o

clean:
	-rm $(patsubst %.cpp,%.o,$(wildcard $(ODIR)*.cpp)) 
	-rm $(EXECUTABLE) $(EXECUTABLE)_debug
	
