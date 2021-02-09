import matplotlib.pyplot as pyplot
from mpl_toolkits.mplot3d import Axes3D


fig = pyplot.figure()
ax = Axes3D(fig)
x = []
y = []
P = []
f_plot = open('Test_rho.txt', 'r')
for line in f_plot:
    line_split = line.split()
    x.append(float(line_split[0]))
    y.append(float(line_split[1]))
    P.append(float(line_split[2]))
ax.invert_xaxis()
ax.scatter(x, y, P)
ax.set_xlabel('x-cord')
ax.set_ylabel('y-cord')
ax.set_zlabel('rho')
pyplot.show()
