import numpy as np
import matplotlib.pyplot as plt

t_ray, u_ray, RK4_ray, euler_ray = np.loadtxt("problem7/data.dat").T

plt.plot(t_ray, RK4_ray, ".-", label="RK4")
plt.plot(t_ray, euler_ray, ".-", label="Euler")
plt.plot(t_ray, u_ray, "k.--", label="u(t)")
plt.legend()
plt.xlabel("t []")
plt.ylabel("u []")
plt.savefig("p7.png", dpi=200)
