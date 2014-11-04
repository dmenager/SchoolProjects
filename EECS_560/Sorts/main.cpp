#include <cstdlib>
#include <iomanip>
#include <climits>
#include <algorithm>
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <cmath>

void bottomUpSort(int n, int a[], int b[]);
void bottomUpMerge(int a[], int iLeft, int iRight, int iEnd, int b[]);
void insertionSort(int a[], int size);
void columnSort(int a[], int size);
void cs(int** a, int r, int s);
void print(int** a, int r, int s);
void print(int* a, int s);
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
    int kVal = 0;
    for(int q = 0; q < s[i]; q++)
    {
      int isIncomplete = false;
      int z = 0;
      int* col = getCol(copy, r, q);

      for(int j = rVal; j < r && z < r; j++)
      {
        for(int k = kVal; k <s[i]; k++)
        {
          if( z != r)
          {
            listMat[j][k] = col[z];
            z++;
          }
          else if(col[z - 1] != listMat[j][k])
          {
            //start using the next column.
            kVal = k;
            isIncomplete = true;
            break;
          }
        }
  
        if(!isIncomplete)
        {
          kVal = 0;
          //go to next row
          rVal = j + 1;
        }
      }
      delete [] col;
    }

    //sort the values in each column
    cs(listMat, r, s[i]);
    
    //untranspose
    int** copy2 = dupa(listMat, r, s[i]);
    
    int q = 0;
    int jVal = 0;
    for(int k = 0; k < s[i]; k++)
    {
      int* row;
      int z = 0;

      for(int j = jVal; j < r && q <= r; j++)
      {
        if(z == s[i] || z == 0)
        {
          z = 0;
          row = getRow(copy2, q, s[i]);
          q++;
        }

        listMat[j][k] = row[z];
        
        //if we need wrap current row around to next column
        if(j == r - 1 && z != s[i] - 1)
        {
          k++;
          j = 0;
          while(z != s[i] - 1)
          {
            z++;
            listMat[j][k] = row[z];
            j++;
          }
          jVal = j;
          k--;
          break;
        }

        z++;
        jVal = 0;
      }
      delete [] row;
    }
    cs(listMat, r, s[i]);
    
    //make shift array 
    int** shift = new int*[r];
    for(int j = 0; j < r; j++)
    {
      shift[j] = new int[s[i] + 1];
    }
  
    for(int j = 0; j < r; j++)
    {
      for(int k = 0; k < s[i]; k++)
      {
        shift[j][k] = listMat[j][k];
      }
    }
    
    for(int j = 0; j < r; j++)
    {
      shift[j][s[i]] = INT_MAX;
    }
    
    //Shift floor(r/2) positions down
    for(int k = s[i] - 1; k >= 0; k--)
    {
      for(int j = r - 1; j >= 0; j--)
      {
        //decide if i need to go to next column
<<<<<<< Updated upstream
        if(j + floor((double) r/2) - 1 >= (r - 1))
        {
          //move to next column
          shift[(int)(j + floor((double) r / 2)) % (r - 1) - 1][k + 1] = shift[j][k];
        }
        else
        {
          shift[(int)(j + floor((double) r/2)) % r][k] = shift[j][k];
=======
        if(j + floor((double)r/2) > r)
        {
          //move to next column
          shift[(int)(j + floor((double) r / 2)) % (r - 1)][k + 1] = shift[j][k];
        }
        else
        {
          shift[(int)(j + floor((double) r/2)) % (r - 1)][k] = shift[j][k];
>>>>>>> Stashed changes
        }
      }
    }

    for(int j = 0; j < floor((double) r / 2); j++)
    {
      shift[j][0] = INT_MIN;
    }

    //Sort the values in each column
    cs(shift, r, s[i] + 1);
    
    //unshift
    for(int k = 0; k < s[i] + 1; k++)
    {
      for(int j = 0; j < r; j++)
      {
        //we want nothing to do with those -infs
        if(k == 0 && j == 0)
        {
          j = (j + floor((double) r / 2));
        }

        if(j - floor((double) r / 2) < 0)
        {
          int idx = (int)(j - floor((double) r / 2));

          //get positive mod
          while(idx < 0)
          {
            idx += r;
          }
          //move to previous column
          idx =  idx % r;
          shift[idx][k - 1] = shift[j][k];
        }
        else
        {
          shift[(int)(j - floor((double) r / 2)) % (r - 1)][k] = shift[j][k];
        }
      }
    }

    //copy new shift to listMat
    
    for(int j = 0; j < r; j++)
    {
      for(int k = 0; k < s[i]; k++)
      {
        listMat[j][k] = shift[j][k];
      }
    }

    print(listMat, r, s[i]);

    for(int j = 0; j < r; j++)
    {
      delete [] listMat[j];
      delete [] copy[j];
      delete [] shift[j];
      delete [] copy2[j];
    }
    delete [] listMat;
    delete [] copy;
    delete [] copy2;
    delete [] shift;

    shift = NULL;
  }
}

bool verify()
{
  int set[23] =  {2, 626, 222, 346, 341, 895, 23, 8,3, 10, 52, 16, 342, 1, 9, 21, 5, 52, 62, 8222, 3421, 33, 42};
  int set2[23] =  {2, 626, 222, 346, 341, 895, 23, 8,3, 10, 52, 16, 342, 1, 9, 21, 5, 52, 62, 8222, 3421, 33, 42};
  int set4[30] =  {2, 626, 222, 346, 341, 895, 23, 8,3, 10, 52, 16, 342, 1, 9, 21, 5, 52, 62, 8222, 3421, 33, 42, 37, 35, 82, 63, 73, 22, 13};
  
  int randSet[180];

  //ONLY FOR VERIFICATION
  srand(time(NULL));

  for(int i = 0; i < 180; i++)
  {
    int n = rand() % 4 * 180 - 2 * 180;

    randSet[i] = n;
  }

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

  collumnSort(randSet, 180);
 
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

void print(int** a, int r, int s)
{
  cout << "Rows = " << r << " Columns = " << s << endl << endl;;
  for(int i = 0; i < r; i++)
  {
    for(int j = 0; j < s; j++)
    {
      cout << std::setw(10) << a[i][j];
    }
    cout << endl;
  }
  cout << "-----------------------------------------------------------" << endl;
}

void print(int* a, int s)
{
  cout << "Size = " << s << endl << endl;
  for(int i = 0; i < s; i++)
  {
    cout << std::setw(10) << a[i];
  }
  cout << endl;
  cout << "-------------------------------" << endl;
}
