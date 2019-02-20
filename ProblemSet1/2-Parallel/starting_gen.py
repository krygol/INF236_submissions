import numpy as np 

p = [1,2,4,8,16,32,128,256,512] 
n = [1024,2048,4096,8192,16382,32768,65536]

for N in n:
    A = np.around(np.random.rand(N, N)) 
    np.savetxt("start_%d" % N, A.astype(int), fmt='%i', delimiter = " ")

