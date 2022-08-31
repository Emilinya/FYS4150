import numpy as np
import matplotlib.pyplot as plt

def u(x):
    return 1 - (1 - np.exp(-10))*x - np.exp(-10*x)
u = np.vectorize(u)

x_rays, y_rays = [], []
with open("problem7/data.dat", "r") as datafile:
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
    abs_log_err = np.log10(np.abs(u(x_ray) - y_ray))
    plt.plot(x_ray, abs_log_err, label=f"n={len(x_ray)}")
plt.legend()
plt.xlabel("x []")
plt.ylabel("log(y) []")
plt.title("Absolute error of numeric solution")
plt.savefig("p8_abs.png", dpi=200)
plt.clf()

for x_ray, y_ray in zip(x_rays, y_rays):
    abs_log_err = np.log10(np.abs(u(x_ray) - y_ray)/np.abs(u(x_ray)))
    plt.plot(x_ray, abs_log_err, label=f"n={len(x_ray)}")
plt.legend()
plt.xlabel("x []")
plt.ylabel("log(y) []")
plt.title("Relative error of numeric solution")
plt.savefig("p8_rel.png", dpi=200)
