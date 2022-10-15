import os
import numpy as np
import matplotlib.pyplot as plt

def plot_broad():
    def sort_func(filename):
            return float(filename.split("f")[1].replace(".dat", ""))
    datafiles = sorted([file for file in os.listdir('problem9') if "_f" in file], key=sort_func, reverse=True)

    plt.figure(tight_layout=True)
    for datafile in datafiles:
        with open(f"problem9/{datafile}", "r") as file:
            total_particles = int(file.readline().split(": ")[1])
        wV_ray, trapped_ray = np.loadtxt(f"problem9/{datafile}", skiprows=1).T
        plt.plot(wV_ray, trapped_ray/total_particles, ".-", label=f"f={sort_func(datafile)}", markersize=4)
    plt.plot([0.59, 0.59], [0, 1], "k--", label="narow scan range", linewidth=1)
    plt.plot([0.79, 0.79], [0, 1], "k--", linewidth=1)
    plt.xlabel("$\omega_V$ [MHz]")
    plt.ylabel("$N_{trapped}/N  _{total}$ []")
    plt.legend()
    plt.savefig("imgs/p9_broad_scan.svg")
    plt.clf()

def plot_narrow():
    def plot_part(datafile, label, color):
        with open(f"problem9/{datafile}", "r") as file:
            total_particles = int(file.readline().split(": ")[1])
        wV_ray, trapped_ray, stds = np.loadtxt(f"problem9/{datafile}", skiprows=1).T
        t_avg = trapped_ray/total_particles
        t_std = stds/total_particles
        plt.fill_between(wV_ray, t_avg - t_std, t_avg + t_std, color=color, alpha=0.4)
        plt.plot(wV_ray, t_avg, '.-', color=color, label=label, markersize=4)

    colors = plt.rcParams['axes.prop_cycle'].by_key()['color']
    plt.figure(tight_layout=True)
    plot_part("data_narrow_no_interactions.dat", "interactions disabled", colors[0])
    plot_part("data_narrow_interactions.dat", "interactions enabled", colors[1])
    plt.xlabel("$\omega_V$ [MHz]")
    plt.ylabel("$N_{trapped}/N  _{total}$ []")
    plt.legend()
    plt.savefig("imgs/p9_narrow_scan.svg")
    plt.clf()

plot_broad()
plot_narrow()

