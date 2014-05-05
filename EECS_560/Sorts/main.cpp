#include <algorithm>
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstddef>

void bottomUpSort(int n, int a[], int b[]);
void bottomUpMerge(int a[], int iLeft, int iRight, int iEnd, int b[]);
void insertionSort(int a[], int size);
void columnSort(int a[], int size);
void cs(int** a, int r, int s);
int** dupa(int** a, int r, int s);
int* getCol(int** a, int r, int s);
int* getRow(int** a, int r, int s);

bool verify();

using namespace std;

int main(int argc, char* argv[])
{
  if(verify())
    cout << "bueno!\n";
  return 0;
}

void insertionSort(int a[], int size)
{
  int i, j, tmp;

  for(i = 1; i < size; i++)
  {
    j = i;
    while(j > 0 && a[j - 1] > a[j])
    {
      tmp = a[j];
      a[j] = a[j - 1];
      a[j - 1] = tmp;
      j--;
    }
  }
}

void bottomUpSort(int n, int a[], int b[])
{

  for(int width = 1; width < n; width = 2 * width)
  {
    for(int i = 0; i < n; i = i + 2 * width)
    {
      bottomUpMerge(a, i, min(i + width, n), min(i + 2 * width, n), b);
    }

    for(int i = 0; i < n; i++)
    {
      a[i] = b[i];
    }
  }
}

void bottomUpMerge(int a[], int iLeft, int iRight, int iEnd, int b[])
{
  int i0 = iLeft;
  int i1 = iRight;

  for(int j = iLeft; j < iEnd; j++)
  {
    if(i0 < iRight && (i1 >= iEnd || a[i0] <= a[i1]))
    {
      b[j] = a[i0];
      i0++;
    }
    else
    {
      b[j] = a[i1];
      i1++;
    }
  }
}

void collumnSort(int a[], int size)
{
  int s[4] = {2, 3, 5, 10}; //s is the number of columns
  for(int i = 0; i < 4; i++)
  {
    //generate matrix
    int r = size / s[i];
    int** listMat = new int*[r];
    
    for(int j = 0; j < r; j++)
    {
      listMat[j] = new int[s[i]];
    }

    int l = 0;
    for(int j = 0; j < r; j++)
    {
      for(int k = 0; k < s[i]; k++)
      {
        listMat[j][k] = a[l];
        l++;
      }
    }
    
    //sort the values in each column
    cs(listMat, r, s[i]);
    
    //Transpose: Access the values in column major order and place them back
      //into the aray in row major order
    int** copy = dupa(listMat, r, s[i]);
    
    int rVal = 0;
    for(int q = 0; q < s[i]; q++)
    {
      int z = 0;
      int* col = getCol(copy, r, q);

      for(int j = rVal; j < r && z < r; j++)
      {
        for(int k = 0; k <s[i]; k++)
        {
          listMat[j][k] = col[z];
          z++;
        }
        //go to next row
        rVal = j + 1;
      }
      delete [] col;
    }

    //sort the values in each column
    cs(listMat, r, s[i]);
    
    //untranspose
    int** copy2 = dupa(listMat, r, s[i]);
    int cVal = 0;
    for(int q = 0; q < r; q++)
    {
      int z = 0;
      int* row = getRow(copy2, q, s[i])

      for(int j = 0; j < s[i]; j++)
      {
        for(int k = cVal; k < r && z < s; k++)
        {
          listMat[k][j] = row[z];
          z++;
        }
        //go to next column
        cVal = k + 1;
      }
    }
    //Shift floor(r/2) positions down 
    //Sort the values in each column
    cs(listMat, r, s[i]);
    //Unshift
    
    for(int j = 0; j < r; j++)
    {
      delete [] listMat[j];
      delete [] copy[j];
    }
    delete [] listMat;
    delete [] copy;
    delete [] copy2;
  }
}

bool verify()
{
  int set[23] =  {2, 626, 222, 346, 341, 895, 23, 8,3, 10, 52, 16, 342, 1, 9, 21, 5, 52, 62, 8222, 3421, 33, 42};
  int set2[23] =  {2, 626, 222, 346, 341, 895, 23, 8,3, 10, 52, 16, 342, 1, 9, 21, 5, 52, 62, 8222, 3421, 33, 42};
  int set4[23] =  {2, 626, 222, 346, 341, 895, 23, 8,3, 10, 52, 16, 342, 1, 9, 21, 5, 52, 62, 8222, 3421, 33, 42};
  insertionSort(set, 23);

  for(int i = 0; i < 22; i++)
  {
    if(set[i] > set[i + 1])
    {
      cout << "insertion Sort does not work\n";
      return false;
    }
  }

  int worker[23];
  bottomUpSort(23, set2, worker);

  for(int i = 0; i < 22; i ++)
  {
    if(set2[i] > set2[i + 1])
    {
      cout << "merge sort does not work\n";
      return false;
    }
  }

  collumnSort(set4, 23);
 
  return true;
}

int** dupa(int** a, int r, int s)
{
  int** c = new int*[r];

  for(int i = 0; i < r; i++)
  {
    c[i] = new int[s];
  }

  for(int i = 0; i < r; i++)
  {
    for(int j = 0; j < s; j++)
    {
      c[i][j] = a[i][j];
    }
  }

  return c;
}

void cs(int** a, int r, int s)
{
  for(int j = 0; j < s; j++)
  {
    //grab correct column
    int* col = getCol(a, r, j);
      
    //sort column
    insertionSort(col, r);

    //overwrite
    for(int n = 0; n < r; n++)
    {
      a[n][j] = col[n];
    }

    delete [] col;
  }
}

int* getCol(int** a, int r, int s)
{
  //grab correct column
  int* col = new int[r];
  for(int m = 0; m < r; m++)
  {
    col[m] = a[m][s];
  }
  return col;
}

int* getRow(int** a, int r, int s)
{
  //grab correct row
  int* row = new int[s];

  for(int m = 0; m < s; m++)
  {
    row[m] = a[r][m];
  }

  return row;
}
