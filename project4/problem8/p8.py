import numpy as np
import matplotlib.pyplot as plt

T_rays = []
Cv_rays = []
X_rays = []

def plot_L(L):
    filename = f"problem8/data-L{L}.dat"
    with open(filename, "r") as datafile:
        repeats, nTs = [int(pair.split("=")[1]) for pair in datafile.readline().split(", ")]
        eps_grid = np.zeros((repeats, nTs))
        absm_grid = np.zeros_like(eps_grid)
        Cv_grid = np.zeros_like(eps_grid)
        X_grid = np.zeros_like(eps_grid)
        for i in range(repeats):
            T_ray, eps_ray, absm_ray, Cv_ray, X_ray = np.loadtxt(datafile, skiprows=1, max_rows=nTs).T
            eps_grid[i, :] = eps_ray
            absm_grid[i, :] = absm_ray
            Cv_grid[i, :] = Cv_ray
            X_grid[i, :] = X_ray

    eps_mean = np.mean(eps_grid, axis=0)
    eps_std = np.std(eps_grid, axis=0)
    absm_mean = np.mean(absm_grid, axis=0)
    absm_std = np.std(absm_grid, axis=0)
    Cv_mean = np.mean(Cv_grid, axis=0)
    Cv_std = np.std(Cv_grid, axis=0)
    X_mean = np.mean(X_grid, axis=0)
    X_std = np.std(X_grid, axis=0)

    T_rays.append(T_ray)
    Cv_rays.append(Cv_mean)
    X_rays.append(X_mean)

    def plot_w_std(xs, means, stds, label):
        color = plot_w_std.colors[0]
        plot_w_std.colors = plot_w_std.colors[1:]
        plt.fill_between(xs, means - stds, means + stds, color=color, alpha=0.4, edgecolor=color)
        plt.plot(xs, means, ".-", color=color, label=label)
    plot_w_std.colors = plt.rcParams['axes.prop_cycle'].by_key()['color']

    plt.figure(tight_layout=True)
    plot_w_std(T_ray, eps_mean, eps_std, "$\\left<\\epsilon\\right>$")
    plot_w_std(T_ray, absm_mean, absm_std, "$\\left<|m|\\right>$")
    plot_w_std(T_ray, Cv_mean, Cv_std, "$C_V$")
    # plot_w_std(T_ray, X_mean, X_std, "$\\chi$")
    plt.ylabel("quantities []")
    plt.xlabel("T [$J/k_B$]")
    ymin, _ = plt.ylim()
    plt.ylim(ymin, 3)
    plt.legend()
    plt.savefig(f"imgs/p8_L={L}.svg")

Ls = [40, 60, 80, 100]
for L in Ls:
    plot_L(L)

def get_critical_temps(rays, name):
    critical_temps = []
    plt.figure(tight_layout=True)
    colors = plt.rcParams['axes.prop_cycle'].by_key()['color']
    for color, T_ray, ray, L in zip(colors, T_rays, rays, Ls):
        plt.plot(T_ray, ray, label=f"L={L}", color=color)

        com = np.sum(T_ray*ray)/np.sum(ray)

        plt.axvline(com, color=color, linestyle="--")
        critical_temps.append(com)
    plt.ylabel(f"{name} []")
    plt.xlabel("T [$J/k_B$]")
    plt.legend()
    plt.savefig(f"imgs/p8_{name}.svg")

    print(critical_temps)
    print(np.diff(critical_temps))

get_critical_temps(Cv_rays, "Cv")
get_critical_temps(X_rays, "X")
