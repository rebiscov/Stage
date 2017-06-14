import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import sys
    
if len(sys.argv) < 2:
    print("Use: ./" + sys.argv[0] + " filename")
    exit(1) 

lines = []
with open(sys.argv[1]) as file:
    lines = file.readlines()

num = []
for line in lines:
    num.append(list(map(int,line.split())))

print(num)
    
    
time = [0,1,2,3]
value = [0,2,4,5]

plt.step(time, value, where='post')

plt.show()

