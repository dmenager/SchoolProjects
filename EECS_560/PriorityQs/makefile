CPP=g++
CFLAGS=-g -Wall

priorityq.out:main.o minmax.o heap.o
	$(CPP) $(CFLAGS) main.o minmax.o heap.o -o priorityq.out

main.o: main.cpp
	$(CPP) $(CFLAGS) -c main.cpp

minmax.o: minmax.h minmax.cpp
	$(CPP) $(CFLAGS) -c minmax.h minmax.cpp

heap.o: heap.h heap.cpp
	$(CPP) $(CFLAGS) -c heap.h heap.cpp

.PHONY:clean
clean:
	rm *.o *.gch priorityq.out
