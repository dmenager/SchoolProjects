#include "heap.h"
#include <iostream>
#include <math.h>
#include <cstdlib>

using namespace std;

heap::heap(int arr[], int len)
{

  A[0] = INV;
  length = len;
  for(int i = 1; i < len; i++)
  {
    A[i] = arr[i - 1];
  }
  comps = 0;

  BuildHeap();
}

heap::~heap() { }

void heap::BuildHeap()
{
  for(int i = ((length))/ 5; i > 0; i--)
  {
    pushDown(i, false);
  }
}

int heap::DeleteMin()
{
  int temp = A[1];
  //delete root
  A[1] = A[length - 1];
  length--;
  pushDown(1, true);

  return temp;
}

int heap::delMin()
{
  int temp = A[1];
  A[1] = A[length - 1];
  length --;
  pDown(1, true);
  return temp;
}


void heap::pushUp()
{
  int val = A[length - 1];

  int pos = length - 1;

  comps++;
  while((val < A[pos / 2]) && (pos > 1))
  {
    A[pos] = A[pos / 2];
    pos = pos / 2;
  }

  A[pos] = val;
}

void heap::pushDown(int idx, bool isCounting)
{
  int val = A[idx], pos = idx;

  int k = 0;
  bool isFound = false;
  
  do
  {
    if(isCounting)
    {
      comps++;
    }

    k = 5 * (pos - 1)  + 0 + 1;
    
    for(int i = 1; i < 5; i++)
    {
      //get the next child
      int test = 5 * (pos - 1) + i + 1;
      if(A[k] > A[test])
      {
        k = test;
      }
    }
    
    if(isCounting)
    {
      comps++;
      //i don't count bounds checiing as a comparison
    }
    if(k <= length - 1 && val > A[k])
    {
      A[pos] = A[k];
      
      pos = k;
    }
    else
    {
      //correct pos has been found
      A[pos] = val;
      isFound = true;
    }
  }while(!isFound);

  cout << "\n\n";
}

void heap::pDown(int idx, bool isCounting)
{
  int pos = idx;

  int k = 0;
  //bool isFound = false;
  do
  {
    if(isCounting)
      comps++;
    
    if(A[2 * pos] < A[2 * pos + 1])
    {
      k = 2 * pos;
    }
    else
    {
      k = 2 * pos + 1;
    }
    
    A[pos] = A[k];
    pos = k;
  }while((2 * pos <= length - 1) && ((2 * pos + 1) <= length - 1));

  pushUp();
}

void heap::Sort()
{
  //int olength = length;
  for(int i = length - 1; i > 0; i--)
  {
    int num;
    if((num = DeleteMin()) != INV)
    {
      A[i] = num;
    }
  }
/*
  if(j < 5)
  {
    cout << "Comparrisons no pushup: " << comps << "\n";
  }
  else
  {
    cout << "Comparisons with pushup: " << comps << "\n";
  }
  */
}
