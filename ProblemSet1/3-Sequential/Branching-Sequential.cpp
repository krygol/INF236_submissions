/*This is the sequential implementation of the Branch Program 
 * of Problems set1 INF236 spring 2019 UIB 

Input: bp.txt assignment

file1: state the branching program 

file2: boolean assignment of length n 


Output: return bool val(P,a) 

*/

/* Copyright 2019 Konstantin Rygol

Permission is hereby granted, free of intge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

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
#include <math.h>
#include <sstream>
#include <string>
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

bool ret_bool(int c){
    bool bo;
    if (c == '1')
        bo = 1;
    else
        bo = 0;
    return bo;
    }
   


int main(int argc, char *argv[]){
    //cout << argc << endl;
    if (argc != 3){
        cout << "give input as bp.txt assignment" << endl;
        cout << "aboarting program" << endl;
        return 0;
    }
 
    //Input parameters
    std::string file1 = argv[1];
    std::string file2 = argv[2];

    //Initalize memory 
    //First file
    //Need to get first n,m,k,r read in bp.txt 
    string line;
    ifstream is(file1);
    
    getline(is,line);
    int n = stoi(line); //number of variables 

    getline(is,line);
    int m = stoi(line); //length of branching program

    getline(is,line);
    int k = stoi(line); // number of elements in S 

    getline(is,line);
    int r = stoi(line); // number of elements in F

    cout << "n: " << n << " k: " << k << " m: " << m << " r: " << r << endl;
    //Allocate the memory to store the sets
    int **C = create2DArray<int>(k*k,3); //holds the assoziative operation
    int **I_ch = create2DArray<int>(m,2); //holds second and third of the  triplets 
    int *i_ind = new int[m]; //holds first of the  triplets 
    int *f = new int[r];  //holds the set of final elements 
    int *s = new int[n];  //Holds the elements  
    int *a = new int[n];  //holds start assignment 

    //cout << "memory allocation is fine" << endl;

    //get the data
    for (int i = 0; i < k; i++){
        getline(is,line);
        s[i] = stoi(line);
        
    }


    //cout << " S is ok" << endl;  

    for (int i = 0;i < k * k; i++){
        getline(is,line);
        std::istringstream buf(line);
        std::istream_iterator<std::string> beg(buf), end;
        std::vector<std::string> tokens(beg, end); // done!
        for(int j = 0; j < 3; j++){
            C[i][j] = stoi(tokens[j]);
        }
    }
    //cout << "Function is ok" << endl;
    for (int i = 0; i < r; i++){
        getline(is,line);
        f[i] = stoi(line);
        
    }
   // cout << "Final elements ok" << endl;
    
    for (int i = 0; i < m; i++){
        getline(is,line);
        std::istringstream buf(line);
        std::istream_iterator<std::string> beg(buf), end;
        std::vector<std::string> tokens(beg, end); // done!
        i_ind[i] = stoi(tokens[0]); 
        //cout << line << endl;
        //total number 
        for(int j = 1;j < 3; j++){
            I_ch[i][j] = stoi(tokens[j]); //assume that tokens is a 2d array
        }
    }
    //cout << "Triplets ok" << endl;
    
    //Read in file2 
    ifstream is2(file2);
    for (int i = 0; i < n; i++){
        getline(is2,line);
        a[i] = stoi(line);
    }

    //Close File streams 
    is.close();
    is2.close();

    cout << "read in finished" << endl;

    //Allocate memory used for calculations
    
    //if we use tree we might have to extend it if tree is not a full tree/does 
    //hold 2^h elements
    //get height of tree
    double log_height = log2((double) m);
    int h = (int) ceil(log_height);

    //It holds the whole memory for a tree like structure
    //int *I_1_m = new int[(int) pow(2.0,ceil(log_height))];  //holds the I we want to add up 
    
    
    int *I_1_m = new int[m];  //holds the I we want to add up 
    //print the triplets 
    //for (int i = 0; i < n ; i++){
      //  cout << i_ind[i] <<" "<< I_ch[i][0] << " " << I_ch[i][1]<< endl;
    //}


    for(int j = 0; j < m ; j++){
        I_1_m[j] = s[I_ch[j][a[i_ind[j]]]]; //check this again
        cout << " a[j]: " << a[j] <<" I_1_m[j]: " << I_1_m[j] << " ";
    }
    cout << endl;
    for(int i = 0; i < n;i++){
        cout << " s[i]: " << s[i] << "\t";
    } 

    cout << endl;
    cout << endl;

    cout << "I_1_m in finished" << endl;
    cout << "h: " << h << endl;
    // now we need to add up all I_1_m by the assosiative operation given in C 
    //look up the two neighbouring elements and add them up we use a tree like structure
    //so we sum up first all pairs (I_1_m[0]+I_1_m[1])+ ... + (I_1_m[m-2]+ I_1_m[m-1])
    //the allows us an easier parallelization after wards 
    // we will work in place savign the elements always as far left as possible
    
    //find the correct matrix element first compare the first summand against first 
    //col of matrix. After it matches look for second summand in second col.
    int l,j;
    int power = 1;
    int sum;
    for(int k = 0; k < h; k++){//height og tree start at lowest layer  
        cout << "m/power+1: " << m/power+1 << endl;
        for(l = 0, j = 0; l < m/power+1 && 2*j < m/power + 1; l += 2, j++){ //width of tree layer 
            int summ_ind_1 = l;
            int summ_ind_2 = l+1;
            int ind1;//=0;
            int ind2;//=0;
            
            //cout << "I_1_m[summ_ind_1]: " << I_1_m[summ_ind_1] << endl;
            //cout << "I_2_m[summ_ind_2]: " << I_1_m[summ_ind_2] << endl;
            
            //compare these to S to find the indices 
            for (int i = 0 ; i < n ; i++)
                if (s[i] == I_1_m[summ_ind_1]){
                    ind1 = i;
                    break;
                }
            for (int i = 0 ; i < n ; i++)
                if (s[i] == I_1_m[summ_ind_2]){
                    ind2 = i;
                    break;
                }

            cout << " ind1 " << ind1 << " ind2 " << ind2 << endl;
            sum = C[ind1 * k + ind2][2]; 
            I_1_m[j] = sum;
            cout << sum << endl;
        }
        power = power * 2;
        cout << "Iteration " << k << " is finished" << endl;
        cout << endl;
    
    }
    
    //check if the sum is in F
    int result = 0;
    for(int i = 0; i < r; i++){
        if (f[i]  == sum)
            result = 1;
    }

    cout << "val(P,a) is: " << result << endl;
    return 0;

}

