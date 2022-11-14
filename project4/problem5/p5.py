import numpy as np
import matplotlib.pyplot as plt

def plot_T(T):
    eps_o, absm_o = np.loadtxt(f"problem5/data-T{T}-ordered.dat").T
    eps_u, absm_u = np.loadtxt(f"problem5/data-T{T}-unordered.dat").T

    idx = np.arange(1, len(eps_o)+1)

    cum_eps_o = np.cumsum(eps_o)/idx
    cum_absm_o = np.cumsum(absm_o)/idx

    cum_eps_u = np.cumsum(eps_u)/idx
    cum_absm_u = np.cumsum(absm_u)/idx

    plt.figure(tight_layout=True)
    plt.axvline(1000, color="k", linestyle="--")
    plt.plot(idx, cum_eps_o, label="$\\left<\\epsilon\\right>$ ordered")
    plt.plot(idx, cum_eps_u, label="$\\left<\\epsilon\\right>$ unordered")
    plt.plot(idx, cum_absm_o, label="$\\left<|m|\\right>$ ordered")
    plt.plot(idx, cum_absm_u, label="$\\left<|m|\\right>$ unordered")

    plt.legend()
    plt.xlabel("cycles []")
    plt.ylabel("quantities []")
    plt.savefig(f"imgs/p5_T={T}.svg")
    plt.clf()

plt.rcParams['font.size'] = '14'

plot_T(1)
plot_T(2.4)
