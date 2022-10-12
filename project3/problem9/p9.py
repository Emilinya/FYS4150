import os
import numpy as np
import matplotlib.pyplot as plt

def sort_func(filename):
        return float(filename.split("f")[1].replace(".dat", ""))
datafiles = sorted([file for file in os.listdir('problem9') if "_f" in file], key=sort_func, reverse=True)

plt.figure(tight_layout=True)
for datafile in datafiles:
    with open(f"problem9/{datafile}", "r") as file:
        total_particles = int(file.readline().split(": ")[1])
    wV_ray, trapped_ray = np.loadtxt(f"problem9/{datafile}", skiprows=1).T
    plt.plot(wV_ray, trapped_ray/total_particles, label=f"f={sort_func(datafile)}")
plt.xlabel("$\omega_V$ [MHz]")
plt.ylabel("$N_{trapped}/N  _{total}$ []")
plt.legend()
plt.savefig("imgs/p9_broad_scan.svg")
