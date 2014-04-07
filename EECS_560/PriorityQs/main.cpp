#define SIZE 51
#define INV 1000000

#include "minmax.h"
#include "heap.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>

void print(MinMax& h);
void print(heap& h);

using namespace std;
int main(int argc, char* argv[])
{
  int n[5] = {25000, 50000, 100000, 200000, 400000};

  srand(time(NULL));
  MinMax mmheap;

  
  for(int j = 0; j < 5; j++)
  {
    for(int i = 1; i < SIZE; i++)
    {
      int val = -2 * n[j] + (rand() % ((2 * n[j]) - (-2 * n[j]) + 1));

      mmheap.Insert(val);
    }
    heap h(mmheap.A, SIZE);
    h.BuildHeap();
    cout << "Min Max Heap:\n";
    print(mmheap);
    cout << "\n";
    cout << "Min 5-Heap:\n";
    print(h);
  }

  return 0;
}

void print(MinMax& h)
{
  for(int i = 1; i < h.last; i++)
  {
    cout << " | " << h.A[i];
  }
  cout << " |\n";
}

void print(heap& h)
{
  h.Sort();
}
