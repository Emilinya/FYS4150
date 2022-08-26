import numpy as np
import matplotlib.pyplot as plt

x_ray, y_ray = np.loadtxt("data.dat").T
plt.plot(x_ray, y_ray)
plt.savefig("p2.png", dpi=200)