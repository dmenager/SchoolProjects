#define SIZE 51
#define INV 1000000

#include "minmax.h"
#include "heap.h"
#include "skew_heap.h"
#include "Timer.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>

#define TESTS 5

void print(MinMax& h);
void print(heap& h);
void print(skew_heap& h);

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

  int testArr[30] = {-244, 214, 202, 277, 883, 900, 88, -728, 180, -321, 
                      222, 523, -43, 52, 823, -256, -2, 12, 67, 241,
                      788, 23, 355, 178, -4, 1, 502, -31, -14, 41};

  verifyHeap(testArr);
  verifyMinMax(testArr);
  verifySkew(testArr);

  cout << "Testing Complete. Running timing tests.\n";
  
  //MinMax
  srand(5);

  test0(n[0]);
  test0(n[1]);
  test0(n[2]);
  test0(n[3]);
  test0(n[4]);

  //D-Heap
  srand(5);

  test1(n[0]);
  test1(n[1]);
  test1(n[2]);
  test1(n[3]);
  test1(n[4]);
  
  //Skew
  srand(5);

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

void print(skew_heap& h)
{
  h.levelOrder();
}
void test0(int n)
{
  cout << "\n\n";
  cout << "****************************************************************************\n";
  cout << "**********************          MIN MAX HEAP          **********************\n";
  cout << "****************************************************************************\n";
  int ins = 0, del = 0, avgD = 0, avgI = 0;
  for(int k = 0; k < 20; k++)
  {
  MinMax h(6 * n + 1);
  Timer t;
  
  for(int i = 0; i < n; i++)
  {
    h.Insert(rand() % (4 * n) - (2 * n));
  }

  int M =rand() % (3 * n) + (2 * n) ;
  
  t.start();  
  for(int i = 0; i < M; i++)
  {
    double x = ((double) rand() / RAND_MAX);

    if(x < (double)1/2)
    {
        h.DeleteMin();
        del++;
    }
    else
    {
      int y = rand() % (4 * n) - (2* n);
      h.Insert(y);
      ins++;
    }
  }
  avgD += del;
  avgI += ins;
  double res = t.stop();
  printf("Test #: %d\tM-val: %d\tPerformance: %5.3fs\n", k, M, res);
  printf("\tTotal insertions: %d\tTotal DeleteMins: %d\n", ins, del);
  }

  printf("Average DeleteMins: %10.3f\tAverage Inserts: %10.3f\n", (double)avgD/20, (double)avgI/20);
}

void test1(int n)
{
  cout << "\n\n";
  cout << "****************************************************************************\n";
  cout << "**************************          5HEAP          *************************\n";
  cout << "****************************************************************************\n";
  int ins = 0, del = 0, avgD = 0, avgI = 0;
  for(int k = 0; k < 20; k++)
  {
  Timer t;
  int* A = new int[n];

  for(int i = 0; i < n; i++)
  {
    A[i] = rand() % (4 * n) - (2 * n) ;
  }
  
  heap h(A, n + 1, 6 * n + 1);

  int M = rand() % (3 * n) + (2 * n) ;
  
  t.start();  
  for(int i = 0; i < M; i++)
  {
    double x = ((double) rand() / RAND_MAX);

    if(x < (double)1/2)
    {
        h.DeleteMin();
        del++;
    }
    else
    {
      int y = rand() % (4 * n) - (2* n);
      h.Insert(y);
      ins++;
    }
  }
  avgD += del;
  avgI += ins;
  double res = t.stop();

  printf("Test #: %d\tM-val %d\tPerformance: %5.3fs\n", k, M, res);
  printf("\tTotal insertions: %d\tTotal DeleteMins: %d\n", ins, del);
  delete [] A;
  }

  printf("Average DeleteMins: %10.3f\tAverage Inserts: %10.3f\n", (double)avgD/20, (double)avgI/20);
}

void test2(int n)
{
  cout << "\n\n";
  cout << "****************************************************************************\n";
  cout << "************************          SKEW HEAP          ***********************\n";
  cout << "****************************************************************************\n";
  int ins = 0, del = 0, avgD = 0, avgI = 0;
  for(int k = 0; k < 20; k++)
  {
  skew_heap s;
  Timer t;
  
  for(int i = 0; i < n; i++)
  {
    s.Insert(rand() % (4 * n) - (2 * n));
  }

  int M = rand() % (3 * n) + (2 * n);
  
  t.start();  
  for(int i = 0; i < M; i++)
  {
    double x = ((double) rand() / RAND_MAX);

    if(x < (double)1/2)
    {
        s.DeleteMin();
        del++;
    }
    else
    {
      int y = rand() % (4 * n) - (2* n);
      s.Insert(y);
      ins++;
    }
  }
  avgD += del;
  avgI += ins;
  double res = t.stop();
  
  printf("Test #: %d\tM-val: %d\tPerformance: %5.3fs\n", k, M, res);
  printf("\tTotal insertions: %d\tTotal DeleteMins: %d\n", ins, del);
  }

  printf("Average DeleteMins: %10.3f\tAverage Inserts: %10.3f\n", (double)avgD/20, (double)avgI/20);
}

void verifyHeap(int arr[])
{
  heap h(arr, 31, 6 * 31);
  printf("Original heap:\n");
  print(h);
  cout <<"\n";

  cout << "Insert\n";
  h.Insert(1);
  print(h);
  cout <<"\n";

  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);

  cout <<"\n";
  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);
  cout <<"\n";
  
  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);
  cout <<"\n";
   
  cout << "Insert\n";
  h.Insert(2);
  print(h);
  cout <<"\n";
  
  cout << "Insert\n";
  h.Insert(6);
  print(h);
  cout <<"\n";


 
  cout << "Insert\n";
  h.Insert(3);
  print(h);
  cout <<"\n";



  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);
  cout <<"\n";
  
  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);
  cout <<"\n";

  cout << "Insert\n";
  h.Insert(5);
  print(h);
  cout <<"\n";


}

void verifyMinMax(int arr[])
{
  MinMax h(6 * 31);
  
  for(int i = 0; i < 30; i++)
  {
    h.Insert(arr[i]);
  }
  
  printf("Original MinMax heap:\n");
  print(h);
  cout <<"\n";


  cout << "Insert\n";
  h.Insert(1);
  print(h);
  cout <<"\n";

  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);
  cout <<"\n";

  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);
  cout <<"\n";
  
  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);
  cout <<"\n";
   
  cout << "Insert\n";
  h.Insert(2);
  print(h);
  cout <<"\n";
  
  cout << "Insert\n";
  h.Insert(6);
  print(h);
  cout <<"\n";


 
  cout << "Insert\n";
  h.Insert(3);
  print(h);
  cout <<"\n";



  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);
  cout <<"\n";
  
  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);
  cout <<"\n";

  cout << "Insert\n";
  h.Insert(5);
  print(h);
  cout <<"\n";
}

void verifySkew(int arr[])
{
  skew_heap h;
  
  for(int i = 0; i < 30; i++)
  {
    h.Insert(arr[i]);
  }

  printf("Original heap:\n");
  print(h);
  cout <<"\n";

  cout << "Insert\n";
  h.Insert(1);
  print(h);
  cout <<"\n";

  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);
  cout <<"\n";

  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);
  cout <<"\n";
  
  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);
  cout <<"\n";
   
  cout << "Insert\n";
  h.Insert(2);
  print(h);
  cout <<"\n";
  
  cout << "Insert\n";
  h.Insert(6);
  print(h);
  cout <<"\n";


 
  cout << "Insert\n";
  h.Insert(3);
  print(h);
  cout <<"\n";



  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);
  cout <<"\n";
  
  cout << "Delete Min\n";
  h.DeleteMin();
  print(h);
  cout <<"\n";

  cout << "Insert\n";
  h.Insert(5);
  print(h);
  cout <<"\n";
}
