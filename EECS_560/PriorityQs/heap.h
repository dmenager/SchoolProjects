#ifndef HEAP_H
#define HEAP_H

#define INV 1000000

#include <math.h>

class heap
{
  public:
    heap(int arr[], int len, int safety);
    virtual ~heap();
    void BuildHeap();
    int DeleteMin();
    void Insert(int key);
    void Sort();
  private:
    void pushUp();
    void pushDown(int idx, bool isCounting);
    void pDown(int idx, bool isCounting);
    int delMin();
    void bubbleUp();

  private:
    int* A;
    int length;
    int comps;
};
#endif
