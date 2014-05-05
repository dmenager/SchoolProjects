CPP=g++
CFLAGS=-g -Wall

sorts.out: main.o
	$(CPP) $(CFLAGS) main.o -o sorts.out

main.o: main.cpp
	$(CPP) $(CFLAGS) -c main.cpp

.PHONY:clean
clean:
	rm *.o *.gch sorts.out
