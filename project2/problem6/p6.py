import numpy as np
import matplotlib.pyplot as plt


def plot_discretization(n):
    plt.figure(constrained_layout=True)
    x_vals = np.linspace(0, 1, n+1)
    v_vals = np.zeros_like(x_vals)
    with open(f"problem6/data_{n}.dat", "r") as datafile:
        for line in datafile:
            lda, *vec = line.split()
            v_vals[1:-1] = np.array(vec, dtype=float)
            plt.plot(x_vals, v_vals, ".-", label=f"$\\lambda={lda}$")
    plt.legend()
    plt.savefig(f"p6_{n}.png", dpi=200)


plot_discretization(10)
plot_discretization(100)
plot_discretization(1000)
