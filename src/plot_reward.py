import numpy as np
import math
import time
import matplotlib
from matplotlib import pyplot as plt
from matplotlib import animation

matplotlib.rcParams.update({'font.size': 12})

#Variable


#read the matrix from the file
A = np.loadtxt('Output_Reward.txt', skiprows=2) 
#for more complex text loading, use numpy.genfromtxt (skiping other rows for example)
#print (A)
#time.sleep(1)




#averaging the reward and depth to plot smoothly
n_av = 1000
B = np.zeros((int(len(A[:,0])/n_av)-1,3))

for i in range(0, len(B)):
    B[i,0] = A[(i+1)*n_av,0]
    for j in range(i*n_av,(i+1)*n_av):
        B[i,1] = B[i,1] + A[j,1]
        B[i,2] = B[i,2] + A[j,2]
    B[i,1] = B[i,1]/n_av
    B[i,2] = B[i,2]/n_av
    



#plot the reward and depth in linear scale
fig, ax1 = plt.subplots(1)
ax1.plot(B[:,0],B[:,1],'b-', linewidth=2.0)
ax1.set_xlabel('Iteration N_it')
ax1.set_ylabel('Reward V', color='b')
ax1.tick_params('y', colors='b')

ax2 = ax1.twinx()
ax2.plot(B[:,0],B[:,2], 'r-', linewidth=2.0)
ax2.set_ylabel('Depth d', color='r')
ax2.set_ylim([0, max(B[:,2])*1.1])
ax2.tick_params('y', colors='r')

fig.tight_layout()
plt.savefig('reward.png')
plt.show()


		
		
#plot the reward and depth in logarithm scale
fig, ax1 = plt.subplots(1)
ax1.semilogx(B[:,0],B[:,1], 'b-', linewidth=2.0)
ax1.set_xlabel('Iteration N_it')
ax1.set_ylabel('Reward V', color='b')
ax1.tick_params('y', colors='b')

ax2 = ax1.twinx()
ax2.semilogx(B[:,0],B[:,2], 'r-', linewidth=2.0)
ax2.set_ylabel('Depth d', color='r')
ax2.set_ylim([0, max(B[:,2])*1.1])
ax2.tick_params('y', colors='r')

fig.tight_layout()
plt.savefig('reward_log.png')
plt.show()
