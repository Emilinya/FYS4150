import numpy as np
import matplotlib.pyplot as plt

def get_analytic_v(n, i):
    d = 2 * n
    a = -1 * n
    eigenval = d + 2 * a * np.cos((i + 1) * np.pi / n)
    eigenvec = np.sin((np.arange(n-1) + 1) * (i + 1) * np.pi / n)
    normalized_eigenvec = eigenvec / np.sqrt(np.sum(eigenvec**2))

    return eigenval, normalized_eigenvec

def plot_discretization(n):
    plt.figure(figsize=(7, 5), constrained_layout=True)
    x_vals = np.linspace(0, 1, n+1)
    v_vals = np.zeros_like(x_vals)
    with open(f"problem6/data_{n}.dat", "r") as datafile:
        for i, line in enumerate(datafile):
            lda, *vec = (float(v) for v in line.split())
            v_vals[1:-1] = vec
            plt.plot(x_vals, v_vals, ".-", label=f"numeric, $\\lambda={lda:.3f}$")
            ana_lda, ana_vec = get_analytic_v(n, i)
            v_vals[1:-1] = np.sign(vec[3]) * np.sign(ana_vec[3]) * ana_vec
            plt.plot(x_vals, v_vals, "--", label=f"analytic, $\\,\\lambda={ana_lda:.3f}$")
    plt.xlabel("$x/L$ []")
    plt.ylabel("$v/\\left<v\\right>}$ []")
    plt.legend()
    plt.savefig(f"p6_{n}.png", dpi=200)

plot_discretization(10)
plot_discretization(100)
