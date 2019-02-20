/*This is the parallel implementation of the Branch Program 
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
#include <mpi.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

    
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

    MPI_Init(NULL,NULL);

    int p;
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 
    cout << "rank: " << rank << " out of " << p << " is here " << endl;
    int k;
    int m;
    int r;
    int **I_ch = NULL;
    int **C = NULL;
    int *I_1_m = NULL;
    int *i_ind = NULL;
    int *f = NULL;
    int *s = NULL;
    int *a = NULL;
    if ( rank == 0){
        //Input from file
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
        m = stoi(line); //length of branching program

        getline(is,line);
        k = stoi(line); // number of elements in S 

        getline(is,line);
        r = stoi(line); // number of elements in F

        cout << "n: " << n << " k: " << k << " m: " << m << " r: " << r << endl;
        //Allocate the memory to store the sets
        C = create2DArray<int>(k*k,3); //holds the assoziative operation
        I_ch = create2DArray<int>(m,2); //holds second and third of the  triplets 
        i_ind = new int[m]; //holds first of the  triplets 
        f = new int[r];  //holds the set of final elements 
        s = new int[k];  //Holds the elements  
        a = new int[n];  //holds start assignment 

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
                cout << C[i][j] << endl;
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
    }
    //cout << " rank " << rank << "reached after read in" << endl;

    //Allocate memory used for calculations
    if (m%p != 0 && rank == 0){
        cout << "m % p != 0" <<endl;
        return 0;
    }
    //cout << "rank "<< rank <<" before bcast is fine" << endl;
    //Broadcast C , k ,m to all ranks
    MPI_Bcast(&k,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&m,1,MPI_INT,0,MPI_COMM_WORLD);
    
    int m_rank = m/p;
    int rank_add_element = m % p;
    if (rank < rank_add_element)    
        m_rank ++;

    int *m_rank_all = new int[p]; 
    int *m_rank_sum = new int[p]; 
    for (int i = 0; i < p; i++){
        if (rank < rank_add_element)
            m_rank_all[i] = m/p + 1;
        else 
            m_rank_all[i] = m/p;
    }


    m_rank_sum[0]=0;
    for (int i = 1 ; i < p; i++){
        m_rank_sum[i] = m_rank_sum[i - 1] + m_rank_all[i-1];
        cout << " rank " << i <<" m_rank_all " << m_rank_all[i] << endl;
        cout << " rank " << i <<" m_rank_sum " << m_rank_sum[i] << endl;
    }
  
    if (rank == 0){
        for(int i = 0; i < p ; i++){
            cout << " rank " << i <<" m_rank_all " << m_rank_all[i] << endl;
            cout << " rank " << i <<" m_rank_sum " << m_rank_sum[i] << endl;
        }
    }
    
    if (rank != 0){
        cout << " k*k rank 0" << k*k << endl;
        C = create2DArray<int>(k*k,3); //holds the assoziative operation
        s = new int[k];  //Holds the elements  
    }
    cout << " rank " << rank << "reached after alloc C" << endl;

    //if we use tree we might have to extend it if tree is not a full tree/does 
    //hold 2^h elements
    //get height of tree
    double log_height_rank = ceil(log2((double) m_rank));
    int elem_rank = (int) ceil(pow(2,log_height_rank));
    int h_rank = (int) ceil(log_height_rank);
    cout << "rank " << rank << " h_rank: " << h_rank << " elem_rank" << elem_rank<< endl;
    
    if (rank == 0){
        I_1_m = new int[m];  //holds the I the operation works on 
        //It holds the whole memory for a tree like structure
        //int *I_1_m = new int[(int) pow(2.0,ceil(log_height))];  //holds the I we want to add up 
        for(int j = 0; j < m ; j++){
            I_1_m[j] = I_ch[j][a[j]]; 
            cout << " a[j]: " << a[j] <<" I_1_m[j]: " << I_1_m[j] << " ";
        }
        cout << endl;
    
         cout << "I_1_m in finished" << endl;
    }
    cout << "rank "<< rank <<" before bcast C is fine" << endl;
   
    MPI_Bcast(&C[0][0],k*k*3,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&s[0],k,MPI_INT,0,MPI_COMM_WORLD);
            
    cout << "rank "<< rank <<" after bcast C is fine" << endl;
    
    //scatter I_1_m to I_1_m_rank
    int *I_1_m_rank = new int[elem_rank];  //holds the I we want to add up 
    cout << "rank " << rank << " elem_rank " << elem_rank << endl;
    cout << "rank "<< rank <<" before scatter is fine" << endl;
    MPI_Scatterv(I_1_m,m_rank_all,m_rank_sum,MPI_INT,I_1_m_rank,
             m_rank_all[rank], MPI_INT,0,MPI_COMM_WORLD);

    auto start = system_clock::now();

    cout << "rank " << rank << " after scatter is fine" << endl;
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

    for(int k = 0; k < h_rank; k++){//height of tree start at lowest layer  
        cout << "m_rank/power: " << m_rank/power + 1<< endl;
        for(l = 0, j = 0; l < m_rank/power + 1&& j * 2 < m_rank/power + 1; l += 2, j++){ //width of tree layer 
            int summ_ind_1 = l;
            int summ_ind_2 = l+1;
            if (rank==0){
                //cout << rank <<" l " << l << " l+1 " << l+1 << endl;        
                cout << "I_1_m[summ_ind_1]: " << I_1_m_rank[summ_ind_1] << endl;
                cout << "s[u]" << s[3] << endl;
                cout << "I_2_m[summ_ind_2]: " << I_1_m_rank[summ_ind_2] << endl;
            }
            int ind1=0;
            int ind2=0;

            //get the index for the first factor repr row in C-Matrix
             for (int i = 0 ; i < k ; i++){
                  if (s[i] == I_1_m_rank[summ_ind_1]){
                     ind1 = i;
                     break;
                  }
             }

            //get the index for the second factor repr col in C-Matrix
             for (int i = 0 ; i < k ; i++){
                  if (s[i] == I_1_m_rank[summ_ind_2]){
                    ind2 = i;
                    break;
                   }
             }
            
            sum = C[ind1 * k + ind2][2]; 
            I_1_m_rank[j] = sum;
            cout << rank << " l " << l << " ind1 " << ind1 << " ind2 " << ind2 
                << " sum " << sum << endl; 
        }
        power = power * 2;
        cout << endl;

    }
    //So now we have all the results of the sub trees we now need to execute the 
    //addition on the reults we gathered from every rank
    //holds the I we want to add up need it only on rank 0 but it is small
    int *I_sums = new int[p];  
    //MPI_Gather the first elements of all I_1_m_rank into I_sums
    cout << "rank "<< rank <<" before gather is fine" << endl;
    MPI_Gather(I_1_m_rank, 1 , MPI_INT, I_sums, 1, MPI_INT, 0, MPI_COMM_WORLD); 

    cout << "rank " << rank << " after gather is fine" << endl;
    
    if (rank == 0){
        //reduce tree by executing the above on rank 0 on newly defined memory 
        //get new height 
        double log_height = log2((double) p);
        int h = (int) ceil(log_height);
        power = 1 ;
        for( int k = 0; k < h; k++){
            cout << " h " << h << " p/power " << p/power << endl;
            for( int j = 0; j < p/power/2 ; j++) {
                int summ_ind_1 = j*2;
                int summ_ind_2 = j*2+1;

                int ind1=0;
                int ind2=0;

                for (int i = 0 ; i < k ; i++)
                      if (s[i] == I_sums[summ_ind_1]){
                         ind1 = i;
                         break;
                      }
                for (int i = 0 ; i < k ; i++)
                    if (s[i] == I_sums[summ_ind_2]){
                        ind2 = i;
                        break;
                    }
        
                cout << " ind1 " << ind1 << " ind2 " << ind2 << endl;
                cout << " I_sums[summ_ind_1] " << I_sums[summ_ind_1]  << " I_sums[summ_ind_2] " <<
                    I_sums[summ_ind_2]  << endl;
                
                sum = C[ind1 * k + ind2][2]; 
                I_sums[j] = sum;
                cout << rank <<" l " << j*2 << " l+1 " << j*2+1 <<" "<<sum << endl;
                //cout << sum << endl;
            }
            power = power * 2;
            cout << endl;
        }

        
        //check if the sum is in F
        int result = 0;
        for(int i = 0; i < r; i++){
            if (f[i]  == sum)
                result = 1;
        }

        cout << "val(P,a) is: " << result << endl;
    }
    auto stop = system_clock::now();
    if (rank==0){
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Time taken by function: " << (double) duration.count()/1000 << "seconds" << endl;
    }
    MPI_Finalize();
    return 0;
}

