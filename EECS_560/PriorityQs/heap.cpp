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
    A[i] = arr[i /*- 1*/];
  }
  comps = 0;

  BuildHeap();
}

heap::~heap() { }

void heap::BuildHeap()
{

  //i = parent of the most right leaf
  for(int i = floor(length - 1 / 5); i > 0; i--)
  {
    pDown(i, false);
  }
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
  //while value is less than its parent
  while((val < A[pos / 5]) && (pos > 1))
  {
    A[pos] = A[pos / 5];
    pos = pos / 5;
  }

  A[pos] = val;
}

void heap::pDown(int idx, bool isCounting)
{
  int pos = idx;

  int k = 0;
  do
  {
    if(isCounting)
      comps++;
    
    //assume the smallest is the left most child
    k = 2 * pos;
    
    for(int i = 1; i < 5; i++)
    {
      if( A[k] >  A[k + i])
      {
        k = k + i;
      }
    }
    
    A[pos] = A[k];
    pos = k;
  }while((2 * pos <= length - 1) && ((2 * pos + 1) <= length - 1) && 
    ((2 * pos + 2) <= length - 1) && ((2 * pos + 3) <= length - 1) && ((2 * pos + 4) <= length - 1));

  pushUp();
}

void heap::Sort()
{
  for(int i = length - 1; i > 0; i--)
  {
    int num;
    if((num = delMin()) != INV)
    {
      A[i] = num;
    }
    cout << " | " << num;
  }
  cout << " |\n";

  //cout << "Comparisons: " << comps << "\n";
}
