import numpy as np
import matplotlib.pyplot as pyplot
from mpl_toolkits.mplot3d import Axes3D

fig = pyplot.figure()
ax = Axes3D(fig)
with open('results.dat', 'r') as results:
    data = np.loadtxt(results)
    x, y, f1, f2, f3, f4, f5, f6, f7, f8, f9, T, rho = data.T

ax.invert_xaxis()
ax.scatter(x, y, rho)
ax.set_xlabel('x-cord')
ax.set_ylabel('y-cord')
ax.set_zlabel('rho')
pyplot.show()
