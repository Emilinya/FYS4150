import numpy as np
import matplotlib.pyplot as plt

def plot_T(T):
    eps, _ = np.loadtxt(f"problem6/data-T{T}.dat").T

    mine, maxe = np.min(eps), np.max(eps)
    bins = int((maxe - mine) / 0.01)

    plt.figure(tight_layout=True)
    plt.hist(eps-0.005, bins, density=True)
    plt.axvline(np.mean(eps), color="k", label="mean")
    plt.axvline(np.mean(eps) + np.std(eps), color="k", linestyle="--", label="std")
    plt.axvline(np.mean(eps) - np.std(eps), color="k", linestyle="--")
    plt.xlabel("$\\epsilon$")
    plt.ylabel(f"$p_{{\\epsilon}}(\\epsilon; {T})$")
    plt.title(f"histograms of generated $\\epsilon$ samples, L=20, T={T}")
    plt.legend()
    plt.savefig(f"imgs/p6_T={T}.svg")
    plt.clf()

plot_T(1)
plot_T(2.4)
