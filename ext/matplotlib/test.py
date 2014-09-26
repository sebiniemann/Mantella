from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
from matplotlib import cm
import numpy
from numpy import linspace, meshgrid
from matplotlib.mlab import griddata

def grid(data, resX=100, resY=100):
    x = data[:, 0]
    y = data[:, 1]
    z = data[:, 2]

    xi = linspace(min(x), max(x), resX)
    yi = linspace(min(y), max(y), resY)
    Z = griddata(x, y, z, xi, yi)
    X, Y = meshgrid(xi, yi)
    return X, Y, Z

data = numpy.loadtxt('data.mat')
X, Y, Z = grid(data)

fig = plt.figure(facecolor = 'white')
ax = fig.gca(projection='3d')
ax.view_init(azim = -58, elev = 12)

surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, alpha=0.8, linewidth = 0, cmap=cm.jet)
ax.contour(X, Y, Z, zdir='z', offset=0.96 * Z.min(), cmap=cm.jet)

ax.set_xlabel(r'Parameter ($x_{1}$)')
ax.set_xlim(-5, 5)
ax.set_ylabel(r'Parameter ($x_{2}$)')
ax.set_ylim(-5, 5)
ax.set_zlabel(r'Fitness value ($f$)')



ax.set_zlim(0.96 * Z.min(), Z.max())

cbar = fig.colorbar(surf)
cbar.set_alpha(1)
cbar.draw_all()

plt.tight_layout()
plt.show()

# savefig('figname.png', facecolor=fig.get_facecolor(), transparent=True)
