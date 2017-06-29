import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import sys

if len(sys.argv) < 2:
    print("Use: ./" + sys.argv[0] + " values_file")
    exit(1)

b=False
if len(sys.argv) > 2:
    b=True
    
lines = []
with open(sys.argv[1]) as file:
    lines = file.readlines()

num = []
for line in lines:
    num.append(list(map(float,line.split())))

t = len(num[1])
x = np.linspace(0, t-1, t)

avg_cost=0.
avg_changes=0

plt.xlim(0,t)
for i in range(int(len(num)/3)):
    changes=int(num[3*i][1])
    cost=num[3*i][0]
    avg_cost += cost
    avg_changes += changes
    if not b:
        plt.title('Cost: ' + str(cost) + ', number of changes: ' + str(changes) + '.')
        plt.plot(x, num[3*i+1])
        plt.step(x, num[3*i+2], where ='post')
        plt.show()

n = len(num)/3
print('Average cost = ' + str(avg_cost/n) + ', average number of changes = ' + str(avg_changes/n))
