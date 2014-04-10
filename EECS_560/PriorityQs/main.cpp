#define SIZE 51
#define INV 1000000

#include "minmax.h"
#include "heap.h"
#include "skew_heap.h"
#include "timer.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>

#define TESTS 5

void print(MinMax& h);
void print(heap& h);
void print(skew_heap& h)

//MinMax
void test0(int n);

//D-Heap
void test1(int n);

//Skew
void test2(int n);

void verifyHeap(int arr[]);
void verifyMinMax(int arr[]);
void verifySkew(int arr[]);

using namespace std;
int main(int argc, char* argv[])
{

  //TODO: Throw exception on DeleteMin

  int n[TESTS] ={25000, 50000, 100000, 200000, 400000};

  int testArr[10] = {23, 355, 178, -4, 1, 502, -31, -14, 41, 56};

  verifyHeap(testArr);
  verifyMinMax(testArr);
  verifySkew(testArr);

  cout << "Testing Complete. Running timing tests.\n";
  
  //MinMax
  srand(time(NULL));

  test0(n[0]);
  test0(n[1]);
  test0(n[2]);
  test0(n[3]);
  test0(n[4]);

  //D-Heap
  srand(time(NULL));

  test1(n[0]);
  test1(n[1]);
  test1(n[2]);
  test1(n[3]);
  test1(n[4]);
  
  //Skew
  srand(time(NULL));

  test2(n[0]);
  test2(n[1]);
  test2(n[2]);
  test2(n[3]);
  test2(n[4]);

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

void test0(int n)
{
  MinMax heap(6 * n);
  Timer t;
  int lowest = -2 * n;
  int highest = 2 * n;
  int range = (highest - lowest) + 1;
  
  for(int i = 0; i < n; i++)
  {
    heap.Insert(lowest + int(range * rand() / (RAND_MAX + 1.0)));
  }
  
  int M = lowest + int(range * rand() / (RAND_MAX + 1.0));
  
  t.start;  
  int del = 0;
  int ins = 0;
  for(int i = 0; i < M; i++)
  {
    int x = ((double) rand() / RAND_MAX;

    if(x < 1/2)
    {
        heap.DeleteMin();
        del++;
    }
    else
    {
      int y = rand() % (4 * n) - (2* n);
      heap.Insert(y);
      ins++;
    }
  }

  double res = t.stop();
  printf("sldfk, ", res);
  cout << ins << del << "\n";
}

void test0(int n)
{
  for(int k = 0; k < 20; k++)
  {
  MinMax h(6 * n);
  Timer t;
  int lowest = -2 * n;
  int highest = 2 * n;
  int range = (highest - lowest) + 1;
  
  for(int i = 0; i < n; i++)
  {
    h.Insert(lowest + int(range * rand() / (RAND_MAX + 1.0)));
  }

  lowest = 2 * n;
  highest = 5 * n;
  range = (highest - lowest) + 1;
  int M = lowest + int(range * rand() / (RAND_MAX + 1.0));
  
  t.start;  
  int del = 0;
  int ins = 0;
  for(int i = 0; i < M; i++)
  {
    int x = ((double) rand() / RAND_MAX;

    if(x < 1/2)
    {
        heap.DeleteMin();
        del++;
    }
    else
    {
      int y = rand() % (4 * n) - (2* n);
      heap.Insert(y);
      ins++;
    }
  }

  double res = t.stop();
  printf("Performance: %5.3f\t", res);
  printf("Total insertions: %d\t Total DeleteMins: %d\n", ins, del); 
  }
}

void test1(int n)
{
  for(int k = 0; k < 20; k++)
  {
  Timer t;
  int lowest = -2 * n;
  int highest = 2 * n;
  int range = (highest - lowest) + 1;
  int* A = new int[n];

  for(int i = 0; i < n; i++)
  {
    A[i] = (lowest + int(range * rand() / (RAND_MAX + 1.0)));
  }
  
  heap h(A, 6 * n);

  h.BuildHeap();

  lowest = 2 * n;
  highest = 5 * n;
  range = (highest - lowest) + 1;
  int M = lowest + int(range * rand() / (RAND_MAX + 1.0));
  
  t.start;  
  int del = 0;
  int ins = 0;
  for(int i = 0; i < M; i++)
  {
    int x = ((double) rand() / RAND_MAX;

    if(x < 1/2)
    {
        heap.DeleteMin();
        del++;
    }
    else
    {
      int y = rand() % (4 * n) - (2* n);
      heap.Insert(y);
      ins++;
    }
  }

  double res = t.stop();
  printf("Performance: %5.3f\t", res);
  printf("Total insertions: %d\t Total DeleteMins: %d\n", ins, del); 

  delete [] A;
  }
}

void test2(int n)
{
  for(int k = 0; k < 20; k++)
  {
  skew_heap s;
  Timer t;
  int lowest = -2 * n;
  int highest = 2 * n;
  int range = (highest - lowest) + 1;
  
  for(int i = 0; i < n; i++)
  {
    s.Insert(lowest + int(range * rand() / (RAND_MAX + 1.0)));
  }

  lowest = 2 * n;
  highest = 5 * n;
  range = (highest - lowest) + 1;
  int M = lowest + int(range * rand() / (RAND_MAX + 1.0));
  
  t.start;  
  int del = 0;
  int ins = 0;
  for(int i = 0; i < M; i++)
  {
    int x = ((double) rand() / RAND_MAX;

    if(x < 1/2)
    {
        heap.DeleteMin();
        del++;
    }
    else
    {
      int y = rand() % (4 * n) - (2* n);
      heap.Insert(y);
      ins++;
    }
  }

  double res = t.stop();
  printf("Performance: %5.3f\t", res);
  printf("Total insertions: %d\t Total DeleteMins: %d\n", ins, del);
  }
}

void verifyHeap(int arr[])
{
  
}

void verifyMinMax(int arr[])
{
}

void verifySkew(int arr[])
{
}
