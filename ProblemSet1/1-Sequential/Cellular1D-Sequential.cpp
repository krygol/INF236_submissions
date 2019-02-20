/*This is the sequential implementation of the 1-Dim Cellular Automata 
 * of Problems set1 INF236 spring 2019 UIB 

Input: file1 file2 t

file1: state transformation function of the cellular automation 

file2: specs the initial configuration of the cellular 

t: number of iterations

Output: 

*/

/* Copyright 2019 Konstantin Rygol

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include <vector>

using namespace std;


    
template <typename T>
T** create2DArray(unsigned nrows, unsigned ncols, const T& val = T())
{
   T** ptr = nullptr;
   T* pool = nullptr;
  // try
  // {
       ptr = new T*[nrows];  // allocate pointers (can throw here)
       pool = new T[nrows*ncols]{val};  // allocate pool (can throw here)

       // now point the row pointers to the appropriate positions in
       // the memory pool
       for (unsigned i = 0; i < nrows; ++i, pool += ncols )
           ptr[i] = pool;
       return ptr;
 //  }
//   catch (std::bad_alloc& ex)
//   {
//       delete [] ptr; // either this is nullptr or it was allocated
//       throw ex;  // memory allocation error
//   }
}

template <typename T>
void delete2DArray(T** arr)
{
   delete [] arr[0];  // remove the pool
   delete [] arr;     // remove the pointers
}

bool ret_bool(char c){
    bool bo;
    if (c == '1')
        bo = 1;
    else
        bo = 0;
    return bo;
    }
   


int main(int argc, char *argv[]){
    //cout << argc << endl;
    if (argc != 4){
        cout << "give input as file1 fil2 #iterations" << endl;
        cout << "aboarting program" << endl;
        return 0;
    }
 
    //Input parameters
    std::string file1 = argv[1];
    std::string file2 = argv[2];
    int t = atoi(argv[3]);

    //Initalize memory 
    //First file
    bool **X = create2DArray<bool>(8,3);
    //x[0][0] = 10;  // values of x 
    //free the memory delete2DArray(dPtr);
    bool *y = new bool[8];

    //Need to get n first read in file2
    string line;
    ifstream is2(file2);
    getline(is2,line);
    int n = stoi(line);
    //Second file 
    bool *env = new bool[n]; //this is the evolution at that time

    //get the inital environment
    getline(is2,line);
    for(int i=0;i<n;i++){
        env[i] = ret_bool(line[i]);
    }

   
    //Read in file1 
    ifstream is(file1);
    int i = 0;
    while (getline(is,line)){
        X[i][0] = ret_bool(line[0]);
        X[i][1] = ret_bool(line[1]);
        X[i][2] = ret_bool(line[2]);
        y[i] = ret_bool(line[4]); //line[3] is the \t
        i++;
    }

    //Close File streams 
    is.close();
    is2.close();

    //number of evolutions
    bool *pos = new bool[3];
    pos[0] = env[(n-1)];
    pos[1] = env[0];
    pos[2] = env[1];
    bool buff;

    for(int k = 0; k < t; k++) {
        //For every position in env
        for(int i = 0; i < n;i++){ 
            //compare the values  against the x if x is found return y  
            //this can be done better by using insted of comparissons
            int ind = 0;
            int mult = 4;
            for(int j = 0; j < 3; j++){
                if(pos[j] == 1){
                    ind += mult;
                }
                mult = mult /2; 
            }
            env[i] = y[ind];
            
        
        pos[0] = pos[1];
        pos[1] = pos[2];
        //we always take the element one to the right. This would fail for the n-th element as 
        //the wrapper asks for 0-th element which has already been updated. Therefore we buffer the
        //0-th element of every generation
        if (i == 0){
            buff = env[i];
        }
        if (i == n-1)
            pos[2] = buff;
        else
            pos[2] = env[(i+2) % n];
        }
       
        
        //cout << "Gen: " << k << "\t";
        for(int i =0; i < n ; i++){
            cout << env[i] << " ";
        }
        cout << endl;
        
          
     }
     /*
    //prints out only last generation
     for(int i =0; i < n ; i++){
        cout << env[i] << " ";
     }
        cout << endl;
*/
}

