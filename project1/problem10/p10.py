import numpy as np
import matplotlib.pyplot as plt

N_ray, general_ray, special_ray = np.loadtxt("problem10/data.dat").T
speedup = float(f"{np.average(general_ray/special_ray):.1f}")

plt.plot(N_ray, general_ray, ".-", label="general algorithm")
plt.plot(N_ray, special_ray, ".-", label="special algorithm")
plt.plot(N_ray, speedup*special_ray, ".-", label=f"{speedup}x special algorithm")
plt.legend()
plt.xlabel("N []")
plt.ylabel("t [ms]")
plt.title("Comparison of general and special algorithm")
plt.savefig("p10.png", dpi=200)