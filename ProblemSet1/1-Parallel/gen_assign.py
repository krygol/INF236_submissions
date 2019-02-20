import numpy as np

for k in range(10,21):
    arr = np.zeros(2*2**k)
    arr[2**k] = 1 
    arr = arr.reshape((1,2*2**k))
    np.savetxt("start_%d" % k, arr.astype(int), fmt='%i', delimiter = " ")
