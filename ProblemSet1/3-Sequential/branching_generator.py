import numpy as np 
import sys 
import random as rnd
import itertools as ite

"""give n,m,k,r as command line parameters.
    n = number of variables
    m = length of branaching program 
    k = size of set S 
    r = size of subset F
This python program generates a random branching program to be used in task3 of INF236 2019 
problem set 1
For this exercise m = n S = {0,..,59}  encoding of the alternating group and + is 
an encoding of the multiplication table of this group. 
The only random part are therefore the triplets
    """
#if(len(sys.argv) is not 2):
#    print("Give the 1 start parameters n,m,k,r \n")

n = 60#int(sys.argv[1])
m = n#60#int(sys.argv[2])
k = 60#int(sys.argv[3])
r = 20 #int(sys.argv[1])

print(n)
print(m)
print(k)
print(r)

#generate the + operation 
#all elements are their own inverse 
#we will jsut use a*b mod 59 to obtain the multiplication table
counter = 0

def is_even(sequence):
    my_count = 0
    for i, num in enumerate(sequence, start=1):
        my_count += sum(num>num2 for num2 in sequence[i:]) 
    return not my_count % 2

all_perm = ite.permutations([1,2,3,4,5])
#get number of inversion x > y but x is before y in sequence 
even_perm = []
for perm in all_perm:
    if is_even(perm):
        even_perm.append(perm)

#generate the finte Set S 
S = range(0,k)
for i in range(0,k):
    str1 = ''.join(str(e) for e in even_perm[i]) 
    print(str1)

#print(even_perm)
np_even = np.array(even_perm)

#now create the matrix C
for i in range(0,k):
    for j in range(0,k):
        #multiply both entries a*b means first apply b to e and then a to be 
        be = np_even[i]
        abe = be[np_even[j] - 1]
        str1 = ''.join(str(e) for e in abe)
        print(str(i) + " " + str(j) + " " + str1)
#generate the Finalset it is a subset of S
F = []
for i in range(r):
    F.append(rnd.randint(0,n-1))
    print(''.join(str(e) for e in even_perm[rnd.randint(0,k-1)]))
#sequence of triplets 
for i in range(0,m):
    index = i #rnd.randint(0,k-1)
    I_0 = ''.join(str(e) for e in even_perm[rnd.randint(0,k-1)])
    I_1 = ''.join(str(e) for e in even_perm[rnd.randint(0,k-1)])

    print(str(index) + " " + I_0 + " " + I_1)



