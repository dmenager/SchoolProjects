CPP=g++
CFLAGS=-g -Wall

quash.out: main.o
	$(CPP) $(CFLAGS) main.o -o quash.out

main.o:main.cpp
	$(CPP) $(CFLAGS) -c main.cpp

.PHONY:clean
clean:
	rm *.o quash.out
