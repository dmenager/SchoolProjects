#ifndef HEAP_H
#define HEAP_H

#define SIZE 51
#define INV 1000000

#include <math.h>

class heap
{
  public:
    heap(int arr[], int len);
    virtual ~heap();
    void BuildHeap();
    int delMin();
    void Sort();
  private:
    void pushUp();
    void pDown(int idx, bool isCounting);

  private:
    int A[SIZE];
    int length;
    int comps;
};
#endif
