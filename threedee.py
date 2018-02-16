
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import numpy as np

fig = plt.figure()
ax = fig.gca(projection='3d')

#data
x = np.arange(0,30)
y = np.arange(0,50)
x, y = np.meshgrid(x,y)
#z = np.sqrt(x*y)
z = np.random.rand(50,30)
print type(z)

print len(z[0])
print x
print y
print z


surf = ax.plot_surface(x,y,z,rstride=1,cstride=20,cmap=cm.coolwarm)

ax.set_xlabel("radius R (deg)")
ax.set_ylabel("n iterations")
ax.set_zlabel("<Q(R,n)>")

plt.show()