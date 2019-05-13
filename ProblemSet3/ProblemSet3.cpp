/*This is the Task1 of Problems set3 INF236 spring 2019 UIB 

Input: file1 

file1: n , 4 n*n number of entries each entry in own row 

Output:  validity of Input

*/

/* Copyright 2019 Konstantin Rygol

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include <vector>
#include <stdexcept>

using namespace std;
    
template <typename T>
T** create2DArray(unsigned nrows, unsigned ncols, const T& val = T())
{
   T** ptr = nullptr;
   T* pool = nullptr;
   ptr = new T*[nrows];  // allocate pointers (can throw here)
   pool = new T[nrows*ncols]{val};  // allocate pool (can throw here)

   // now point the row pointers to the appropriate positions in
   // the memory pool
   for (unsigned i = 0; i < nrows; ++i, pool += ncols )
       ptr[i] = pool;
   return ptr;
}

template <typename T>
void delete2DArray(T** arr)
{
   delete [] arr[0];  // remove the pool
   delete [] arr;     // remove the pointers
}

int main(int argc, char *argv[]){
    //cout << argc << endl;
    if (argc != 2){
        throw std::invalid_argument( "Give input as: Inputfile");
    }
 
    //Input parameters
    std::string file1 = argv[1];
    
    string line;
    ifstream is(file1);
    getline(is,line);
    int n = stoi(line);
    int **Grid = create2DArray<int>(2*n,2*n);
    //Read in file 
    for(int i = 0; i < 2*n; i++){ //row
        for(int j = 0 ; j < 2*n; j++) //col
        {
            getline(is,line);
            Grid[i][j] = stoi(line);
        }
    }
    //Close File streams 
    is.close();

    
    //Check that the instance is a valid instance
    //check the rows
    for(int i = 0; i < 2*n; i++){ //row
        int number_black = 0;
        int number_white = 0;
        int tmp1 = 55;
        int tmp2 = 57;
        for(int j = 0 ; j < 2*n; j++) //col
        {
            if(Grid[i][j] == 0){
                number_black ++;
            }
            if(Grid[i][j] == 1){
                number_white ++;
            }
            if (tmp1 == tmp2 && Grid[i][j] == tmp1 && Grid[i][j] != 2){
                 throw std::invalid_argument( "3 times the same number row wise");
            }
            tmp1 = tmp2;
            tmp2 = Grid[i][j];
        }
        //after every row check number of black and white fields
        if (number_black > n|| number_white > n){
            throw std::invalid_argument("Number of black or white fields are greater than n row wise");
        }
    }

    //check the col - just switched the two for loops
    for(int j = 0 ; j < 2*n; j++){ //col
        int number_black = 0;
        int number_white = 0;
        int tmp1 = 55;
        int tmp2 = 55;
        for(int i = 0; i < 2*n; i++) //row
        {
            if(Grid[i][j] == 0){
                number_black ++;
            }
            if(Grid[i][j] == 1){
                number_white ++;
            }
            if (tmp1 == tmp2 && Grid[i][j] == tmp1 && Grid[i][j] != 2){
                throw std::invalid_argument("3 times white or black coloum wise");
            }
            tmp1 = tmp2;
            tmp2 = Grid[i][j];
        }
        //after every col check number of black and white fields
        if (number_black > n || number_white > n){
             throw std::invalid_argument("Number of black or white fields are greater than n coloumn wise");
        }
    }
    cout << "The input is valid" << endl;
    
    //Free the memory 
    delete2DArray(Grid);

    return 1;
    
}

