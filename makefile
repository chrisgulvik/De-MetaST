CC = g++

CFLAGS = -g 

EXECUTABLES: De-MetaST

all: $(EXECUTABLES)

.SUFFIXES: .cpp .o
.cpp.o:
	$(CC) $(CFLAGS) -c $*.cpp

De-MetaST: De-MetaST.o PrimerDriver.o PrimerLib.o
	$(CC) $(CFLAGS) -o $@ *.o
    
#make clean will rid your directory of the executable,
#object files, and any core dumps you've caused
clean:
	rm De-MetaST *.o


