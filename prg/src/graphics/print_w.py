import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import sys
    
if len(sys.argv) < 4:
    print("Use: ./" + sys.argv[0] + " d s filename")
    exit(1) 

lines = []
with open(sys.argv[3]) as file:
    lines = file.readlines()

num = []
for line in lines:
    num.append(list(map(int,line.split())))

d = int(sys.argv[1])
s = int(sys.argv[2])
x = np.linspace(0,d+1,d+2)

for y in num:
    plt.xlim(0,d+1)
    plt.ylim(0,d*s)
    plt.step(x, y + [y[-1]], where='post')
    plt.show()


