#ifndef MINMAX_H
#define MINMAX_H

#define SIZE  51
#define INV 1000000

class MinMax
{
  public:
    int A[SIZE];
    int last;
  public:
    MinMax();
    void Insert(int key);
    void DeleteMin();
    void DeleteMax();
    int Test(int i);
  private:
    void trickleDown(int i);
    void trickleDownMin(int i, int l, int r);
    void trickleDownMax(int i, int l, int r);

    void bubbleUp(int i);
    void bubbleUpMin(int i, int p);
    void bubbleUpMax(int i, int p);

    bool isMin(int i);
    bool hasKids(int i, int l, int r);
    int getLeft(int i);
    int getRight(int i);
    int getParent(int i);
    bool hasParent(int i);
    int smallestOfChildsGrandChilds(int i);
    int largestOfChildsGrandChilds(int i);

    void swap(int& x, int& y);
};
#endif
