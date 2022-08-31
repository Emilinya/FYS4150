import numpy as np
import matplotlib.pyplot as plt

x_ray, u_ray = np.loadtxt("problem2/data.dat").T
plt.plot(x_ray, u_ray, label="u(x)")
plt.legend()
plt.xlabel("x []")
plt.ylabel("y []")
plt.title("Analytic solution of the one-dimensional Poisson equation")
plt.savefig("p2.png", dpi=200)