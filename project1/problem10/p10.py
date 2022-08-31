import numpy as np
import matplotlib.pyplot as plt

N_ray, general_ray, special_ray = np.loadtxt("problem10/data.dat").T
plt.plot(N_ray, general_ray, ".-", label="general algorithm")
plt.plot(N_ray, special_ray, ".-", label="special algorithm")
plt.plot(N_ray, 2*special_ray, ".-", label="2x special algorithm")
plt.legend()
plt.xlabel("N []")
plt.ylabel("t [ms]")
plt.title("Comparison of general and special algorithm")
plt.savefig("p10.png", dpi=200)

print("Speedups:")
print(general_ray/special_ray)