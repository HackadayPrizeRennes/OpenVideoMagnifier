import matplotlib as mpl
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cbook as cbook

from scipy.cluster import vq

data = np.genfromtxt('log', delimiter='.', skip_header=0, skip_footer=0, names=['x', 'y'])
fig = plt.figure()

ax1 = fig.add_subplot(111)

ax1.set_title("debug")    
ax1.set_xlabel('x')
ax1.set_ylabel('y')

ax1.plot(data['x'], data['y'], 'bs', color='r', label='the data')

leg = ax1.legend()

plt.show()
