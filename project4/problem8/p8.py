import numpy as np
import matplotlib.pyplot as plt

def plot_L(L):
    T_ray, eps_ray, absm_ray, Cv_ray, X_ray = np.loadtxt(f"problem8/data-L{L}.dat").T

    plt.figure(tight_layout=True)
    plt.plot(T_ray, eps_ray, label="$\\left<\\epsilon\\right>$")
    plt.plot(T_ray, absm_ray, label="$\\left<|m|\\right>$")
    plt.plot(T_ray, Cv_ray, label="$C_V$")
    plt.plot(T_ray, X_ray, label="$\\chi$")
    plt.legend()
    plt.savefig(f"imgs/p8_L={L}.svg")

for L in [40, 60, 80, 100]:
    plot_L(L)
