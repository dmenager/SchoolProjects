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
    int DeleteMin();
    void Sort();
  private:
    void pushUp();
    void pushDown(int idx, bool isCounting);
    void pDown(int idx, bool isCounting);
    int delMin();

  private:
    int A[SIZE];
    int length;
    int comps;
};
#endif
