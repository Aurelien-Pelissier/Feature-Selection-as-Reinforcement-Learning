import numpy as np
import matplotlib
from matplotlib import pyplot as plt



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
ax2.set_ylabel('Depth d (number of features)', color='r')
ax2.set_ylim([0, max(B[:,2])*1.1])
ax2.tick_params('y', colors='r')
ax2.xaxis.set_major_locator((plt.MaxNLocator(4)))

fig.tight_layout()
plt.title('Average evaluated feature set at each 1000 iteration')
plt.savefig('reward.png')
plt.show()



C = np.loadtxt('Output_Reward2.txt', skiprows=2)

a = A.shape[0] / C.shape[0]

#plot the reward2 and depth2 in linear scale
fig, ax1 = plt.subplots(1)
ax1.plot(C[:,0]*a,C[:,1],'b-', linewidth=2.0)
ax1.set_xlabel('Iteration N_it')
ax1.set_ylabel('Reward V', color='b')
ax1.tick_params('y', colors='b')

ax2 = ax1.twinx()
ax2.plot(C[:,0]*a,C[:,2], 'r-', linewidth=2.0)
ax2.set_ylabel('Depth d (number of features)', color='r')
ax2.set_ylim([0, max(C[:,2])*1.1])
ax2.tick_params('y', colors='r')
ax2.xaxis.set_major_locator((plt.MaxNLocator(4)))

fig.tight_layout()
plt.title('Average recommended best feature set score at each 1000 iteration')
plt.savefig('reward2.png')
plt.show()

print("\n       Best leaf score is ", C[-1,1])

		
"""		
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
"""