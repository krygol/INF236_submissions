/*This is the parallel MPI implementation of the 1-Dim Cellular Automata 
 * of Problems set1 INF236 spring 2019 UIB 

Input: file1 file2 t

file1: state transformation function of the cellular automation 

file2: specs the initial configuration of the cellular 

t: number of iterations

Output: the configuration after t iterations  

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
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    cout << "rank " << rank << " out of " << p << " is here " << endl;
    auto start = system_clock::now();
    //reserve memory to be distributed
    int n_rank;
    int t;
    int rank_add_elements = 0;
   
    //Initalize memory 
    //First file
    //free the memory delete2DArray(dPtr);
    bool *y = new bool[8];
    int *n_rank_all = new int[p]; // stores the n_rank of all ranks
    int *n_rank_sum = new int[p]; // stores the sum of all n_rank up to a point
    bool *env = NULL;
    int n; 
    if(p % 2 == 1){
        cout << "give even mumber of processes.\n" 
        "Program can not handle odd number of processes" <<endl;
        return 0;
        }

    //The read in will be done the rank 0. After the data is in RAM we will distribute it
    if(rank == 0) {
        if (argc != 4){
            cout << "give input as file1 fil2 #iterations" << endl;
            cout << "aboarting program" << endl;
            return 0;
        }
     
        //Input parameters
        std::string file1 = argv[1];
        std::string file2 = argv[2];
        t = atoi(argv[3]);

        //Need to get n first read in file2
        string line;
        ifstream is2(file2);
        getline(is2,line);
        n = stoi(line);
        n_rank = n/p; // this is the floor we have to see what we do with the remainder 
       
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
        //Second file 
        env = new bool[n]; //this is the evolution at that time
        //get the inital environment
        getline(is2,line);
        for(int i=0;i<n;i++){
            env[i] = ret_bool(line[i]);
        }
        //Read in file1 
        ifstream is(file1);
        int i = 0;
        while (getline(is,line)){
            y[i] = ret_bool(line[4]); //line[3] is the \t
            i++;
        }
        cout << " n = " << n << endl;
        cout << " p = " << p << endl;
        cout << " Iterations = " << t << endl;

        //Close File streams 
        is.close();
        is2.close();
        cout << "Read in is fine" << endl; 
    }
    //All ranks need their local y and their part of env=the environment
    //Therefore we need to allocate memory 
    
    //broadcast n to other ranks so they know how big env should be
    MPI_Bcast(&n_rank,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&rank_add_elements,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&t,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(n_rank_sum,p,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(n_rank_all,p,MPI_INT,0,MPI_COMM_WORLD);

    //Data from second file
    bool *env_rank = new bool[n_rank_all[rank]+2]; //part of evolution pluss border elements 
    cout << "rank " << rank << " has n_rank " << n_rank_all[rank] << endl;
    //Distribute all the data 
    
    MPI_Bcast(y,8,MPI_C_BOOL,0,MPI_COMM_WORLD);
    
    //Data from second file
    //use MPI Scatterv have to create and array with all n_rank   
    //to gether it back use gather v
    
    MPI_Scatterv(env,n_rank_all,n_rank_sum,MPI_C_BOOL,&env_rank[1],
            n_rank_all[rank], MPI_C_BOOL,0,MPI_COMM_WORLD);

    env_rank[0] = 0;
    env_rank[n_rank_all[rank]+1] = 0;


    bool *pos = new bool[3];

    for(int k = 0; k < t+1; k++) {

        MPI_Gatherv(&env_rank[1], n_rank_all[rank], MPI_C_BOOL, env, n_rank_all,
          n_rank_sum, MPI_C_BOOL,0,MPI_COMM_WORLD);
/*
        if (rank == 0){
            cout << "Generation: " << k << "\t";
            for(int i = 0; i < n ; i++){
                cout << " " << env[i];
            }
            cout << endl;
        }
  */      
        //we also need to get the left most and right most element 
        //After all calc is done for the iteration send data to your neighbours use torso wrapping
        //this seems to be broken for p>1
        //cout << "rank: " << rank << "before torus send" << endl;
        
        if (rank % 2 == 0){
            MPI_Send(&env_rank[1],1,MPI_C_BOOL,(rank-1+p)%p,0,MPI_COMM_WORLD);
            MPI_Send(&env_rank[n_rank_all[rank] + 0],1,MPI_C_BOOL,(rank+1)%p,1,MPI_COMM_WORLD);
            MPI_Recv(&env_rank[n_rank_all[rank] + 1],1,MPI_C_BOOL,(rank+1)%p,1,MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
            MPI_Recv(&env_rank[0]  ,1,MPI_C_BOOL,(rank-1+p)%p,0,MPI_COMM_WORLD
                    ,MPI_STATUS_IGNORE);
 
        }

        if (rank % 2 == 1){
            MPI_Recv(&env_rank[0],1,MPI_C_BOOL,(rank-1+p)%p,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            MPI_Recv(&env_rank[n_rank_all[rank] + 1],1,MPI_C_BOOL,(rank+1)%p,0,MPI_COMM_WORLD,
              MPI_STATUS_IGNORE);    
            MPI_Send(&env_rank[1],1,MPI_C_BOOL,(rank-1+p)%p,1,MPI_COMM_WORLD);
            MPI_Send(&env_rank[n_rank_all[rank] + 0],1,MPI_C_BOOL,(rank+1)%p,0,MPI_COMM_WORLD);
           
        }
       
        //number of evolutions
        pos[0] = env_rank[0];  
        pos[1] = env_rank[1];
        pos[2] = env_rank[2];


        //For every position in env
        for(int i = 1; i < n_rank_all[rank] + 1;i++){//n-rank is the number of elements we work on 
            //compare the values  against the x if x is found return y  
            int ind = 0;
            int mult = 4;
            for(int j = 0; j < 3; j++){
                if(pos[j] == 1){
                    ind += mult;
                }
                mult = mult /2;
            }
            env_rank[i] = y[ind];
            pos[0] = pos[1];
            pos[1] = pos[2];
            pos[2] = env_rank[i+2];
            
        }

        
           
    }
    //get all env_rank back to rank 0 env using gatherv
    
    MPI_Gatherv(&env_rank[1], n_rank_all[rank], MPI_C_BOOL, env, n_rank_all,
            n_rank_sum, MPI_C_BOOL,0,MPI_COMM_WORLD);
   
/*
     if (rank == 0){
            cout << "Generation: " << t << "\t";
            for(int i = 0; i < n ; i++){
                cout << " " << env[i];
            }
            cout << endl;
        }
        */
   auto stop = system_clock::now();
   if (rank == 0){
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Time taken by function: " << (double) duration.count()/1000 << "seconds" << endl;
    }
    MPI_Finalize();
}
 
