/*This is the parallel implementation of the 2-Dim Cellular Automata 
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
#include <mpi.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

bool ret_bool(char c){
    bool bo;
    if (c == '1')
        bo = 1;
    else
        bo = 0;
    return bo;
    }
   
int main(int argc, char *argv[]){
    MPI_Init(NULL,NULL);

    //number of processes
    int p; 
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    //get rank
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, & rank);
    //cout << "rank: " << rank << " out of " << p << " is here " << endl;
    auto start = system_clock::now();  
    int n_rank;
    int t;
    int n;
    int rank_add_elements;
    int *n_rank_all = new int[p]; // stores the n_rank of all ranks
    int *n_n_rank_all = new int[p]; // stores the n_rank of all ranks *n_rank
    int *n_rank_sum = new int[p]; // stores the sum of all n_rank up to a point
    int *n_n_rank_sum = new int[p]; // stores the sum of all n_rank*n_rank up to a point
    bool *env = NULL;
    if(p % 2 == 1){
        cout << "give even mumber of processes.\n" 
        "Program can not handle odd number of processes" <<endl;
        return 0;
        }
    //Initalize memory 
    //First file
    //free the memory delete2DArray(dPtr);
    bool *y = new bool[512];


   
    //cout << argc << endl;
    if (rank == 0){
        if (argc != 4){
            cout << "give input as file1 fil2 #iterations" << endl;
            cout << "aboarting program" << endl;
            return 0;
        }
     
        //Input parameters
        std::string file1 = argv[1];
        std::string file2 = argv[2];
        t = atoi(argv[3]);

        //Initalize memory 
        //First file
        //free the memory delete2DArray(dPtr);
        y = new bool[512];

        //Need to get n first read in file2
        string line;
        ifstream is2(file2);
        getline(is2,line);
        n = stoi(line);
        cout << " n = " << n << endl;
        cout << " p = " << p << endl;
        cout << " Iterations = " << t << endl;

        //Second file 
        env = new bool[n*n]; //this is the evolution at that time
        //get the inital environment 2D
        for(int i = 0; i < n; i++){ //row
            getline(is2,line);
            for(int j = 0 ; j < n; j++) //col
            {
                env[i * n + j] = ret_bool(line[j]);
            }
        }
        /*
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
        */
        //Read in file1 
        ifstream is(file1);
        int i = 0;
        while (getline(is,line)){
            y[i] = ret_bool(line[10]); //line[9] is the \t
            i++;
        }
         
        //Close File streams 
        is.close();
        is2.close();
        //cout << "Read in is fine" << endl; 
    }
    /*
    if(rank == 0){
        cout << "Initial Environment" << endl;
            for(int i =0; i < n ; i++){
                    for(int j = 0; j < n; j++)
                    {
                        cout << env[i * n + j] << " ";
                    }
                    cout << endl;
                }
            cout << endl;
        }   
    
      cout << endl;

    */
    //broadcast n to other ranks so they know how big env should be
    MPI_Bcast(&n_rank,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&rank_add_elements,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&t,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
    //cout << "rank: " << rank << " First Broadcasts are fine " << endl;
    n_rank = n/p;
    //handling the remainder
    if (n%p != 0){
        //up to this rank all ranks get one element more 
        rank_add_elements = n%p;
        for (int i = 0; i < p; i++){
            n_rank_all[i] = n_rank; // deviation of n_rank    
            if(i < rank_add_elements)
                n_rank_all[i]++;    
        }

    }
    else {
        for(int i = 0; i < p; i++)
            n_rank_all[i] = n_rank;                
    }
    
    //calculate n_rank_sum
    n_rank_sum[0] = 0;
    for(int i = 1; i < p ; i++){
        n_rank_sum[i] = n_rank_sum[i-1] + n_rank_all[i-1];
    }
    //MPI_Barrier(MPI_COMM_WORLD);
    //Data from second file
    //part of evolution plus border elements we split data horizontaly every rank 
    //gets n/p lines of length n. if n%p != 0 some hold n/p + 1 
    bool *env_rank = new bool[(n_rank_all[rank] + 2) *n];
    //cout << "rank: " << rank << "env_rank is fine" << endl;
    //this is the next evolution 
    bool *env_next_rank = new bool[(n_rank_all[rank] + 2) *n];
    bool *env_data = new bool[n*n];
    
    //Distribute the function 
    MPI_Bcast(y,512,MPI_C_BOOL,0,MPI_COMM_WORLD);

    //Data from second file
    //use MPI Scatterv have to create and array with all n_rank   
    //to gether it back use gather v
     
    // does not work as we cannot stretch arrays 

    for (int i=0; i < p ; i++){
        n_n_rank_all[i] = n_rank_all[i]*n;
        n_n_rank_sum[i] = n_rank_sum[i]*n;
    }
   
    MPI_Scatterv(env,n_n_rank_all,n_n_rank_sum,MPI_C_BOOL,&env_rank[n],
            n_n_rank_all[rank], MPI_C_BOOL,0,MPI_COMM_WORLD);
    //positions a 9 element array for the calculation of each element we then need 3 new elements 
    //the matrix we need counts rows first upper right is pos 2, lower left pos 6 in the array. 
    bool *pos = new bool[9];
    int ii,jj;
    for(int k = 0; k < t; k++) {
        
         
        //we also need to get the left most and right most element 
        //After all calc is done for the iteration send data to your neighbours use torso wrapping
        //this seems to be broken for p>1
        //cout << "rank: " << rank << "before torus send" << endl;
        if (rank % 2 == 0){
            MPI_Send(&env_rank[n],n,MPI_C_BOOL,(rank-1+p)%p,0,MPI_COMM_WORLD);
            MPI_Send(&env_rank[n_rank_all[rank] * n],n,MPI_C_BOOL,(rank+1)%p,1,MPI_COMM_WORLD);
            MPI_Recv(&env_rank[0],n,MPI_C_BOOL,(rank+1)%p,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            MPI_Recv(&env_rank[(n_rank_all[rank] + 1) * n],n,MPI_C_BOOL,(rank-1+p)%p,1,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);
           
        }

        if (rank % 2 == 1){
            MPI_Recv(&env_rank[0],n,MPI_C_BOOL,(rank+1)%p,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            MPI_Recv(&env_rank[(n_rank_all[rank] + 1) * n],n,MPI_C_BOOL,(rank-1+p)%p,1,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);    
            MPI_Send(&env_rank[n],n,MPI_C_BOOL,(rank-1+p)%p,0,MPI_COMM_WORLD);
            MPI_Send(&env_rank[(n_rank_all[rank] + 0)*n],n,MPI_C_BOOL,(rank+1)%p,1,MPI_COMM_WORLD);
        }
        
        //For every position in env
        for(ii = 1; ii < n_rank_all[rank] + 1; ii++){ //row
            for(jj = 0; jj < n ; jj++){ //col
                //we always take the element one to the right. 
                //This would fail for the n-th element as 
                if(ii == 1 && jj == 0){
                    //initial the pos array
                    for(int i = 0; i < 3; i++){
                        for(int j = 0; j < 3; j++){
                            pos[3 * i + j] = env_rank[((n + i + 0) % n) * n + (n + j - 1) % n];
                        }
                    }            
                }
                else {
                    for(int i = 0; i < 3; i++){
                        pos[0 + 3 * i] = pos[1 + 3 * i];
                        pos[1 + 3 * i] = pos[2 + 3 * i];
                    }
                        pos[2] = env_rank[((ii - 1 + n) % n) * n + (jj + 1)] % n;
                        pos[5] = env_rank[((ii + 0) % n) * n + (jj + 1) % n];
                        pos[8] = env_rank[((ii + 1 ) % n) * n + (jj + 1) % n];
                }
                int ind = 0;
                int mult = 256;
                for (int j = 0; j < 9; j++){
                    if(pos[j] == 1){
                        ind += mult;
                    }
                    mult = mult /2; 
                }
                
                env_next_rank[ii * n + jj] = y[ind];
           
            }  
           // cout << endl;
        }
        //update the env from env_next_rank
       // cout << "rank " << rank << " Iteration " << k  << " of " << t << " has gone trough" << endl;
        for(int i = 0 ; i < n_rank_all[rank] + 2; i++){
            for(int j = 0; j < n; j++){
                env_rank[i * n + j] = env_next_rank[i * n + j];              
            }  
        }
    }
    MPI_Gatherv(&env_rank[n], n_n_rank_all[rank], MPI_C_BOOL, env, n_n_rank_all,
        n_n_rank_sum, MPI_C_BOOL,0,MPI_COMM_WORLD);

/*
    if(rank ==0){
        cout << "Gatherv after iteration:  " << t  << endl;
        for(int i=0; i < n;i++)
        {
            for(int j=0; j < n; j++){
                cout << env[i *n +j] << "\t";
            }
            cout << endl;
        }
    }
*/
    //Free the memory 
    delete [] y;
    delete [] pos;
    delete [] env;
    delete [] env_rank;
    auto stop = system_clock::now();
    if (rank == 0){
        auto duration = duration_cast<milliseconds>(stop - start); 
        cout << "Time taken by function: " << (double) duration.count()/1000 << "seconds" << endl; 
    }
    MPI_Finalize();
    
    return 0;
}
