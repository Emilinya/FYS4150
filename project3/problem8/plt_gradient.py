import numpy as np
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
from mpl_toolkits.mplot3d.art3d import Line3DCollection

# Code based on https://nbviewer.jupyter.org/github/dpsanders/matplotlib-examples/blob/master/colorline.ipynb

def make_segments(x, y):
	'''
	Create list of line segments from x and y coordinates, in the correct format for LineCollection:
	an array of the form   numlines x (points per line) x 2 (x and y) array
	'''

	points = np.array([x, y]).T.reshape(-1, 1, 2)
	segments = np.concatenate([points[:-1], points[1:]], axis=1)

	return segments

def colorline(x, y, z=None, cmap=plt.get_cmap('viridis'), norm=plt.Normalize(0.0, 1.0), ax=None, **kwargs):
	'''
	Plot a colored line with coordinates x and y
	Optionally specify colors in the array z
	Optionally specify an axis, a colormap, a norm function, and other LineCollection kwargs
	'''

	# Default colors equally spaced on [0,1]:
	if z is None:
		z = np.linspace(0.0, 1.0, len(x))

	# Special case if a single number:
	if not hasattr(z, "__iter__"):  # to check for numerical input -- this is a hack
		z = np.array([z])

	z = np.asarray(z)

	segments = make_segments(x, y)
	lc = LineCollection(segments, array=z, cmap=cmap, norm=norm,  **kwargs)

	if ax is None or ax == plt:
		ax = plt.gca()

	ax.add_collection(lc)

	return lc

def colorline3D(x, y, z, c=None, cmap=plt.get_cmap('viridis'), ax=None, **kwargs):
	'''
	Plot a colored line with coordinates x, y and z
	Optionally specify colors in the array c
	Optionally specify an axis, a colormap, a norm function, and other LineCollection kwargs
	'''

	# Default colors equally spaced on [0,1]:
	if c is None:
		c = np.linspace(0.0, 1.0, len(x))

	# Special case if a single number:
	if not hasattr(c, "__iter__"):  # to check for numerical input -- this is a hack
		c = np.full_like(x, c)

	c = np.asarray(c)

	if ax is None or ax == plt:
		ax = plt.gca()

	# if cmap is a string, create a proper cmap
	try:
		cmap(0)[:2]
	except:
		cmap = plt.get_cmap(cmap)

	step = int(np.ceil(len(x) / 1000))
	for i in range(0, len(x)-step, step):
		ax.plot(x[i:i+step], y[i:i+step], z[i:i+step], c=cmap(c[int(i+step/2)])[:-1], **kwargs)
	rest = min(i+step, len(x)-1)
	ax.plot(x[rest:], y[rest:], z[rest:], c=cmap(c[-1])[:-1], **kwargs)
