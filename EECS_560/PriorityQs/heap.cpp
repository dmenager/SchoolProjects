#include "heap.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

heap::heap(int arr[], int len, int safety)
{
  A = new int[safety];

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
  if(A[length] == INV)
  {
    cout << "INVALID HEAP OPERATION: Heap already empty\n";
    return INV;
  }
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
  //pDown(1, true);
  return temp;
}

int heap::myceil(double num)
{
  int inum = (int)num;
  if(num == (double)inum)
  {
    return inum;
  }
  return inum + 1;
}


void heap::pushUp()
{
  int val = A[length - 1];

  int pos = length - 1;
  double num = (((double)pos - 1)/5);
  int par = myceil(num);

  int k = 5 * (par - 1) + 0 + 2;
  for(int i = 1; i < 5; i++)
  {
    //get the next child
    int test = 5 * (par - 1) + i + 2;
    if(test > length - 1)
      break;
    if(A[k] > A[test])
    {
      k = test;
    }
  }

  val = A[k];
  pos = k;

  double see = ((double)pos - 1)/5;
  par = myceil(see);
  while((val < A[par]) && (pos > 1))
  {
    A[pos] = A[par];
    pos = par;
    see = ((double)pos - 1) / 5;
    par = myceil(see);
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
}

/*
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
*/
void heap::Insert(int key)
{
  length++;
  A[length - 1] = key;
  pushUp();
}

void heap::bubbleUp()
{
  int childIdx = length - 1;
  int temp = A[childIdx];

  while(childIdx > 1 && temp < A[(childIdx)/ 5])
  {
    A[childIdx] = A[(childIdx - 1)/ 5];
    childIdx = (childIdx - 1)/ 5;
  }

  A[childIdx] = temp;
}

void heap::Sort()
{
  for(int i = 0; i < length - 1; i++)
  {
    cout << " | " << A[i + 1];
  }
  cout << " |\n";
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
