import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import sys

if len(sys.argv) < 3:
    print("Use: python3 " + sys.argv[0] + " values_file_1 values_file_2")
    exit(1)

lines1 = []
with open(sys.argv[1]) as file:
    lines1 = file.readlines()

lines2 = []
with open(sys.argv[2]) as file:
    lines2 = file.readlines()    

num1 = []
for line in lines1:
    num1.append(list(map(float,line.split())))

num2 = []
for line in lines2:
    num2.append(list(map(float,line.split())))

t = len(num1[1])
x = np.linspace(0, t-1, t)

plt.xlim(0,t)
for i in range(int(len(num1)/3)):
    plt.title('Cost of the blue line: ' + str(num1[3*i][0]) + ', cost of the green line: ' + str(num2[3*i][0]))
    plt.plot(x, num1[3*i+1])
    plt.step(x, num1[3*i+2], where ='post')
    plt.step(x, num2[3*i+2], where ='post')    
    plt.show()

