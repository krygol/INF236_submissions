import numpy as np

for i in range(512):
    number = bin(i)[2:].zfill(9)
    #what is a neighbour extract neighbour string cut first two and the one in position 4 
    #print(number)
    #print(number[0:4])
    #print(number[4])
    #print(number[5:9])
    neighb = number[0:4] + number[5:9]
    cell = number[4]
    
#    print("left neighb,cell,right neighb "+ str(number[0:4]) +" "+ str(cell) +" "+ str(number[5:9]))
    numb_0 = neighb.count('0')
    numb_1 = neighb.count('1')
    y = 0
    if (numb_1 < 2):
        y = 0
    elif(numb_1 == 2 and cell == '1'):
        y = 1
    elif(numb_1 == 3):
        y = 1
    elif(numb_1 > 3):
        y = 0
    print(str(number) + " " + str(y)) 
