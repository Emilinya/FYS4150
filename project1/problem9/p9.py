import numpy as np
import matplotlib.pyplot as plt

x_rays, y_rays = [], []
with open("problem9/data.dat", "r") as datafile:
    x_ray = []
    y_ray = []
    for line in datafile:
        try:
            x, y = [float(v) for v in line.split(" ")]
            x_ray.append(x)
            y_ray.append(y)
        except:
            x_rays.append(x_ray)
            x_ray = []
            y_rays.append(y_ray)
            y_ray = []

for x_ray, y_ray in zip(x_rays, y_rays):
    plt.plot(x_ray, y_ray, label=f"n={len(x_ray)}")

true_x_ray, true_y_ray = np.loadtxt("problem2/data.dat").T
plt.plot(true_x_ray, true_y_ray, "--k", label="u(x)")

plt.legend()
plt.xlabel("x []")
plt.ylabel("y []")
plt.title("Numeric solutions of the one-dimensional Poisson equation")
plt.savefig("p9.png", dpi=200)
