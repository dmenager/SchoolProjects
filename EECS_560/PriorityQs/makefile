CPP=g++
CFLAGS=-g -Wall

priorityq.out:main.o minmax.o heap.o skew_heap.o Node.o
	$(CPP) $(CFLAGS) main.o minmax.o heap.o skew_heap.o Node.o -o priorityq.out

main.o: main.cpp
	$(CPP) $(CFLAGS) -c main.cpp

minmax.o: minmax.h minmax.cpp
	$(CPP) $(CFLAGS) -c minmax.h minmax.cpp

heap.o: heap.h heap.cpp
	$(CPP) $(CFLAGS) -c heap.h heap.cpp

skew_heap.o: skew_heap.h skew_heap.cpp
	$(CPP) $(CFLAGS) -c skew_heap.h skew_heap.cpp

Node.o: Node.h Node.cpp
	$(CPP) $(CFLAGS) -c Node.h Node.cpp

.PHONY:clean
clean:
	rm *.o *.gch priorityq.out
