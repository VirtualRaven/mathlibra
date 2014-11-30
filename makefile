CC=g++
CFLAGS=  -Wall -std=c++11 -I ./headers/ -fexceptions
ODIR =  ./source/
EXECUTABLE=  calc

all:  $(patsubst %.cpp,%.o,$(wildcard $(ODIR)*.cpp))
	$(CC) $(CFLAGS)  $?  -o $(EXECUTABLE)


.cpp.o: 
	@echo $*
	$(CC) $(CFLAGS) -c  $*.cpp -o $*.o
clean:
	rm $(EXECUTABLE)
	rm $(wildcard $(ODIR)*.cpp) 
