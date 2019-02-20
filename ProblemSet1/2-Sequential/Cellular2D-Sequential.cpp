/*This is the sequential implementation of the 2-Dim Cellular Automata 
 * of Problems set1 INF236 spring 2019 UIB 
 later in the task Conors game of live is implemented

Input: file1 file2 t

file1: state transformation function of the cellular automation 

file2: specs the initial configuration of the cellular 

t: number of iterations

Output: matrix env 2-dim 

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

bool ret_bool(char c){
    bool bo;
    if (c == '1')
        bo = 1;
    else
        bo = 0;
    return bo;
    }
   
int main(int argc, char *argv[]){
    cout << "Hello Program" << endl;
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
    bool **X = create2DArray<bool>(512,9);
    //x[0][0] = 10;  // values of x 
    //free the memory delete2DArray(dPtr);
    bool *y = new bool[512];

    //Need to get n first read in file2
    string line;
    ifstream is2(file2);
    getline(is2,line);
    int n = stoi(line);
    //Second file 
    bool **env = create2DArray<bool>(n,n); //this is the evolution at that time
    bool **env_next = create2DArray<bool>(n,n); //this is the next evolution e

    //get the inital environment 2D
    for(int i = 0; i < n; i++){ //row
        getline(is2,line);
        for(int j = 0 ; j < n; j++) //col
        {
            env[i][j] = ret_bool(line[j]);
        }
    }
    //intial env
    cout << "Initial Environment" << endl;
    for(int i =0; i < n ; i++){
            for(int j = 0; j < n; j++)
            {
                cout << env[i][j] << " ";
            }
            cout << endl;
        }
    cout << endl;
    //Read in file1 
    ifstream is(file1);
    int i = 0;
    while (getline(is,line)){
        for(int j = 0; j < 9; j++){
            X[i][j] = ret_bool(line[j]);
        }
        y[i] = ret_bool(line[10]); //line[9] is the \t
        i++;
    }

    //Close File streams 
    is.close();
    is2.close();
    //for (i=0;i<8;i++)
    //cout << y[i] << endl; 
    

   //number of evolutions

    //replace this by positions a 9 element array each iteration we then need 3 new elements 
    //the matrix we need counts rows first upper right is pos 2, lower left pos 6 in the array. 
    bool *pos = new bool[9];
    
    

    int ii,jj;
    for(int k = 0; k < t; k++) {
        //For every position in env
        for(ii = 0; ii < n;ii++){ //row
            for(jj = 0; jj < n;jj++){ //col
                //we always take the element one to the right. 
                //This would fail for the n-th element as 
                if(ii==0 && jj == 0){
                    //initial the pos array
                    for(int i = 0; i < 3; i++){
                        for(int j = 0; j < 3; j++){
                            pos[3 * i + j] = env[(n + i - 1) % n][(n + j - 1) % n];
                        }
                    }            
                }
                else {
                    for(int i = 0; i < 3; i++){
                        pos[0 + 3 * i] = pos[1 + 3 * i];
                        pos[1 + 3 * i] = pos[2 + 3 * i];
                    }
                        pos[2] = env[(ii - 1 + n) % n][(jj + 1)] % n;
                        pos[5] = env[(ii + 0) % n][(jj + 1) % n];
                        pos[8] = env[(ii + 1 ) % n][(jj + 1) % n];
                }
            //do this mroe efficently using the technique from 1-Seq
                int ind = 0;
                int mult = 256;
                for (int j = 0; j < 9;j++){
                    if(pos[j] == 1){
                        ind += mult;
                    }
                    mult = mult /2; 
                }
                
                env_next[ii][jj] = y[ind];
                cout << ind << " ";
                
           
            }  
            cout << endl;
        }
        //update the env from env_next
        for(int i =0 ; i < n; i++){
            for(int j = 0; j < n; j++){
                env[i][j] = env_next[i][j];              
            }  
        }
        
        cout << endl << "Generation: " << k+1 << endl;
        for(int i =0; i < n ; i++){
            for(int j = 0; j < n; j++)
            {
                cout << env[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        cout << endl;
        cout << endl;
    }
    //Free the memory 
    delete [] y;
    delete [] pos;
    delete2DArray(env);
    delete2DArray(env_next);
    delete2DArray(X);

   return 0;
    
}

