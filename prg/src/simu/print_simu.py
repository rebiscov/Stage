import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import sys

if len(sys.argv) < 2:
    print("Use: ./" + sys.argv[0] + " values_file")
    exit(1)
    
lines = []
with open(sys.argv[1]) as file:
    lines = file.readlines()

num = []
for line in lines:
    num.append(list(map(int,line.split())))

t = len(num[0])
x = np.linspace(0, t-1, t)

plt.xlim(0,t)
plt.plot(x, num[0])
plt.step(x, num[1], where ='post')
plt.show()
