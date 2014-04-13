#include "minmax.h"
#include <cmath>
#include <algorithm>

using namespace std;

MinMax::MinMax(int size)
{
  A = new int[size];
  for(int i = 0; i < size; i++)
  {
    A[i] = INV;
  }

  last = 1;
}

MinMax::~MinMax()
{
  delete [] A;
}

void MinMax::Insert(int key)
{
  A[last] = key;

  bubbleUp(last);

  last++;
}

void MinMax::DeleteMin()
{
  if(last == 1)
  {
    cout << "INVALID HEAP OPERATION: Heap already empty\n"; 
  }
  A[1] = A[last - 1];

  A[last - 1] = INV;

  last--;

  trickleDown(1);
}

void MinMax::DeleteMax()
{
  int m;

  if(A[2] > A[3])
  {
    m = 2;
  }
  else
  {
    m = 3;
  }

  A[m] = A[last - 1];

  A[last - 1] = INV;

  last--;

  trickleDown(m);
}

void MinMax::trickleDown(int i)
{ 
  if(isMin(i))
  {
    trickleDownMin(i, getLeft(i), getRight(i));
  }
  else
  {
    trickleDownMax(i, getLeft(i), getRight(i));
  }
}

void MinMax::trickleDownMin(int i, int l, int r)
{
  l = getLeft(i);
  r = getRight(i);
  if(hasKids(i, l, r))
  {
    int m = smallestOfChildsGrandChilds(i);

    //if m is grandchild of A[i]
    if(getParent(getParent(m)) == i)
    {
      if(A[m] < A[i])
      {
        swap(A[i], A[m]);

        if(A[m] > A[getParent(m)])
        {
          swap(A[m], A[getParent(m)]);
        }
      }
      trickleDownMin(m, l, r);
    }
    else
    {
      if(A[m] < A[i])
      {
        swap(A[i], A[m]);
      }
    }
  }
}

void MinMax::trickleDownMax(int i, int l, int r)
{
  l = getLeft(i);
  r = getRight(i);

  if(hasKids(i, l, r))
  {
    int m = largestOfChildsGrandChilds(i);

    //if m is grandchild of A[i]
    if(getParent(getParent(m)) == i) 
    {
      if(A[m] > A[i])
      {
        swap(A[i], A[m]);
        
        if(A[m] < A[getParent(m)])
        {
          swap(A[m], A[getParent(m)]);
        }
      }

      trickleDownMax(m, l, r);
    }
    else
    {
      if(A[m] > A[i])
      {
        swap(A[i], A[m]);
      }
    }
  }
}

bool MinMax::isMin(int i)
{
  
  int level = floor(log2(i));

  if(level % 2 == 0)
  {
    return true;
  }

  return false;
}

bool MinMax::hasKids(int i, int l, int r)
{
  if(l >= SIZE || A[l] == INV)
    return false;
  if(r >= SIZE || A[r] == INV)
    return false;
  
  return true;
}

bool MinMax::hasParent(int i)
{
  if(floor(i / 2) > 0)
  {
    return true;
  }

  return false;
}

int MinMax::getLeft(int i)
{
  return 2 * i;
}

int MinMax::getRight(int i)
{
  return 2 * i + 1;
}

int MinMax::getParent(int i)
{
  return floor((i / 2));
}


int MinMax::smallestOfChildsGrandChilds(int i)
{
  int lftChild = getLeft(i);
  int min = lftChild;
  int minVal = A[lftChild];

  int rgtChild = getRight(i);

  if(rgtChild >= last)
  {
    return min;
  }

  int rgtMin = A[rgtChild];

  if(rgtMin < minVal)
  {
    min = rgtChild;
    minVal = rgtMin;
  }

  int grandChild = getLeft(lftChild);
  
  if(grandChild >= last)
  {
    return min;
  }
  
  int grandChildVal = A[grandChild];

  if(grandChildVal < minVal)
  {
    min = grandChild;
    minVal = grandChildVal;
  }
  
  int grandChild2 = getRight(lftChild);
  
  if(grandChild2 >= last)
  {
    return min;
  }
  
  int grandChild2Val = A[grandChild2];

  if(grandChild2Val < minVal)
  {
    min = grandChild2;
    minVal = grandChild2Val;
  }
  
  int grandChild3 = getLeft(rgtChild);
  
  if(grandChild3 >= last)
  {
    return min;
  }
  
  int grandChild3Val = A[grandChild3];

  if(grandChild3Val < minVal)
  {
    min = grandChild3;
    minVal = grandChild3Val;
  }
  
  int grandChild4 = getRight(rgtChild);
  
  if(grandChild4 >= last)
  {
    return min;
  }
  
  int grandChild4Val = A[grandChild4];

  if(grandChild4Val < minVal)
  {
    min = grandChild4;
    minVal = grandChild4Val;
  }

  return grandChild4Val < minVal ? grandChild4 : min;
}

int MinMax::largestOfChildsGrandChilds(int i)
{
  int lftChild = getLeft(i);
  int max = lftChild;
  int maxVal = A[lftChild];

  int rgtChild = getRight(i);

  if(rgtChild >= last)
  {
    return max;
  }

  int rgtMax = A[rgtChild];

  if(rgtMax > maxVal)
  {
    max = rgtChild;
    maxVal = rgtMax;
  }

  int grandChild = getLeft(lftChild);
  
  if(grandChild >= last)
  {
    return max;
  }
  
  int grandChildVal = A[grandChild];

  if(grandChildVal > maxVal)
  {
    max = grandChild;
    maxVal = grandChildVal;
  }
  
  int grandChild2 = getRight(lftChild);
  
  if(grandChild2 >= last)
  {
    return max;
  }
  
  int grandChild2Val = A[grandChild2];

  if(grandChild2Val > maxVal)
  {
    max = grandChild2;
    maxVal = grandChild2Val;
  }
  
  int grandChild3 = getLeft(rgtChild);
  
  if(grandChild3 >= last)
  {
    return max;
  }
  
  int grandChild3Val = A[grandChild3];

  if(grandChild3Val > maxVal)
  {
    max = grandChild3;
    maxVal = grandChild3Val;
  }
  
  int grandChild4 = getRight(rgtChild);
  
  if(grandChild4 >= last)
  {
    return max;
  }
  
  int grandChild4Val = A[grandChild4];

  if(grandChild4Val > maxVal)
  {
    max = grandChild4;
    maxVal = grandChild4Val;
  }

  return grandChild4Val > maxVal ? grandChild4 : max;
}


void MinMax::bubbleUp(int i)
{
  //int l = getLeft(i);
  //int r = getRight(i);
  int p = getParent(i);
  if(isMin(i))
  {
    if(hasParent(i) && A[i] > A[p])
    {
      swap(A[i], A[p]);
      bubbleUpMax(p, getParent(p));
    }
    else
    {
      bubbleUpMin(i, p);
    }
  }
  else
  {
    if(hasParent(i) && A[i] < A[p])
    {
      swap(A[i], A[p]);
      bubbleUpMin(p, getParent(p));
    }
    else
    {
        bubbleUpMax(i, p);
    }
  }
}

void MinMax::bubbleUpMin(int i, int p)
{
  if(hasParent(p))
  {
    if(A[i] < A[getParent(p)])
    {
      swap(A[i], A[getParent(p)]);
      bubbleUpMin(getParent(p), getParent(getParent(p)));
    }
  }
}

void MinMax::bubbleUpMax(int i, int p)
{
  if(hasParent(p))
  {
    if(A[i] > A[getParent(p)])
    {
      swap(A[i], A[getParent(p)]);
      bubbleUpMax(getParent(p), getParent(getParent(p)));
    }
  }
}

void MinMax::swap(int& x, int& y)
{
  if(x != y)
  {
    x ^= y;
    y ^= x;
    x ^= y;
  }
}
