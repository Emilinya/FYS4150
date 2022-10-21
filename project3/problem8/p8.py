import os
import numpy as np
from tqdm import tqdm
import matplotlib as mpl
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

from plt_gradient import colorline, colorline3D


def plot_single_z():
    with open("problem8/data_single_movement.dat", "r") as datafile:
        datafile.readline()
        vals = [float(v) for v in datafile.readline().split()]
        r0 = vals[:3]
        v0 = vals[3:6]
        wz = vals[6]
    t_ray, z_ray = np.loadtxt(
        "problem8/data_single_movement.dat", skiprows=2
    ).T

    plt.figure(tight_layout=True)
    plt.plot(t_ray, r0[2]*np.cos(wz*t_ray), "r", label="analytical solution")
    # skip values to avoid dots looking like a full line
    plt.plot(t_ray[::20], z_ray[::20], ".g", label="numerical solution")
    plt.legend()
    plt.title(f"One ion, $r_0={r0}, v_0={v0}$")
    plt.xlabel("$t$ [$\\mu s$]")
    plt.ylabel("$z$ [$\\mu m$]")
    plt.savefig("imgs/p8_single_movement.svg", dpi=200)
    plt.clf()


def add_gradient_label(labels, labelSize, cmaps, t_ray, ax):
    charSize = 0.15 / len("particle 1")
    padding = (0.01 + charSize*max(len(label) for label in labels), 0.015)
    textPadding = labelSize[1]/2

    norm = mpl.colors.Normalize(vmin=np.min(t_ray), vmax=np.max(t_ray))
    for i, (label, cmap) in enumerate(zip(labels, cmaps)):
        y_offset = (1.5*i+1)*labelSize[1]
        axins = ax.inset_axes(
            [1-padding[0]-labelSize[0], 1-padding[1]-y_offset, *labelSize])
        sm = plt.cm.ScalarMappable(cmap=cmap, norm=norm)
        sm.set_array([])
        cbar = plt.colorbar(sm, cax=axins, cmap=cmap, orientation='horizontal')
        cbar.set_ticks([])
        textArgs = (1-padding[0] + textPadding, 1-padding[1]-y_offset, label)
        try:
            ax.text2D(*textArgs, transform=ax.transAxes)
        except:
            ax.text(*textArgs, transform=ax.transAxes)

    # Add ticks to last colorbar
    cbar.set_ticks([np.min(t_ray), np.max(t_ray)])
    cbar.set_ticklabels(
        [f"${round(np.min(t_ray), 5):5g}\\mu s$", f"${round(np.max(t_ray), 5):5g}\\mu s$"])


def add_dots(xrays, yrays, zrays=None):
    # This could probably be done less repetitively, but such is life
    if zrays is None:
        for i, (xray, yray) in enumerate(zip(xrays, yrays)):
            if i == 0:
                plt.plot(xray[0], yray[0], "g.", label="start")
                plt.plot(xray[-1], yray[-1], "r.", label="end")
            else:
                plt.plot(xray[0], yray[0], "g.")
                plt.plot(xray[-1], yray[-1], "r.")
    else:
        for i, (xray, yray, zray) in enumerate(zip(xrays, yrays, zrays)):
            if i == 0:
                plt.plot(xray[0], yray[0], zray[0], "g.", label="start")
                plt.plot(xray[-1], yray[-1], zray[-1], "r.", label="end")
            else:
                plt.plot(xray[0], yray[0], zray[0], "g.")
                plt.plot(xray[-1], yray[-1], zray[-1], "r.")


def plot_double_xy(interactions):
    interStr = "interactions" if interactions else "no_interactions"

    t_ray, x1_ray, y1_ray, _, _, _, _, x2_ray, y2_ray, *_ = np.loadtxt(
        f"problem8/data_double_movement_{interStr}.dat"
    ).T

    limits = 80

    _, ax = plt.subplots(tight_layout=True, figsize=(6, 6))

    labelSize = (0.10, 0.02)
    cmaps = ("winter", "spring")

    # add particle paths with color to show movement over time
    colorline(x1_ray, y1_ray, cmap=cmaps[0], ax=ax)
    colorline(x2_ray, y2_ray, cmap=cmaps[1], ax=ax)
    add_gradient_label(["particle 1", "particle 2"],
                       labelSize, cmaps, t_ray, ax)

    # add dots marking the start and end of paths
    add_dots((x1_ray, x2_ray), (y1_ray, y2_ray))
    plt.legend(loc="upper left")

    ax.set_title(
        f"Two ions, {interStr.replace('_', ' ')}")
    ax.set_xlabel("$x$ [$\\mu m$]")
    ax.set_ylabel("$y$ [$\\mu m$]")
    ax.set_xlim(-limits, limits)
    ax.set_ylim(-limits, limits)
    plt.savefig(f"imgs/p8_double_movement_{interStr}.svg", dpi=200)
    plt.clf()
    ax.clear()


def plot_double_phase(interactions):
    interStr = "interactions" if interactions else "no_interactions"

    t_ray, * \
        data = np.loadtxt(f"problem8/data_double_movement_{interStr}.dat").T
    x1_ray, y1_ray, z1_ray, vx1_ray, vy1_ray, vz1_ray = data[:6]
    x2_ray, y2_ray, z2_ray, vx2_ray, vy2_ray, vz2_ray = data[6:]

    def plot_phase(pos1, pos2, vel1, vel2, dim):
        if dim == "z":
            fig = plt.figure(tight_layout=True, figsize=(6, 6))
        else:
            fig = plt.figure(tight_layout=True)
        ax = fig.add_subplot()

        labelSize = (0.10, 0.02)
        cmaps = ("winter", "spring")
        colorline(pos1, vel1, cmap=cmaps[0], ax=ax)
        colorline(pos2, vel2, cmap=cmaps[1], ax=ax)
        add_gradient_label(["particle 1", "particle 2"],
                           labelSize, cmaps, t_ray, ax)

        plt.xlabel(f"${dim}$ [$\\mu m$]")
        plt.ylabel(f"$v_{dim}$ [m/s]")

        max_pos = max(np.max(pos1), np.max(pos2))
        min_pos = min(np.min(pos1), np.min(pos2))
        max_vel = max(np.max(vel1), np.max(vel2))
        min_vel = min(np.min(vel1), np.min(vel2))

        xpadd = (max_pos - min_pos) * 0.02
        ypadd = (max_vel - min_vel) * 0.02

        plt.xlim(min_pos-xpadd, max_pos+xpadd)
        plt.ylim(min_vel-ypadd, max_vel+ypadd)

        add_dots((pos1, pos2), (vel1, vel2))
        plt.legend(loc="upper left")

        plt.title(
            f"Two ions, {interStr.replace('_', ' ')}")
        plt.savefig(
            f"imgs/p8_double_movement_phase_space_{dim}_{interStr}.svg", dpi=200)
        plt.clf()

    plot_phase(x1_ray, x2_ray, vx1_ray, vx2_ray, "x")
    plot_phase(y1_ray, y2_ray, vy1_ray, vy2_ray, "y")
    plot_phase(z1_ray, z2_ray, vz1_ray, vz2_ray, "z")


def plot_double_3d(interactions):
    interStr = "interactions" if interactions else "no_interactions"

    t_ray, x1_ray, y1_ray, z1_ray, _, _, _, x2_ray, y2_ray, z2_ray, * \
        _ = np.loadtxt(f"problem8/data_double_movement_{interStr}.dat").T

    limits = 80

    fig = plt.figure(tight_layout=True)
    ax = fig.add_subplot(projection='3d')
    ax.set_xlim(-limits, limits)
    ax.set_ylim(-limits, limits)

    labelSize = (0.15, 0.025)
    cmaps = ("winter", "spring")

    colorline3D(x1_ray, y1_ray, z1_ray, cmap=cmaps[0], ax=ax)
    colorline3D(x2_ray, y2_ray, z2_ray, cmap=cmaps[1], ax=ax)
    add_gradient_label(["particle 1", "particle 2"],
                       labelSize, cmaps, t_ray, ax)

    add_dots((x1_ray, x2_ray), (y1_ray, y2_ray), (z1_ray, z2_ray))
    plt.legend(loc="upper left")

    ax.set_xlabel("$x$ [$\\mu m$]")
    ax.set_ylabel("$y$ [$\\mu m$]")
    ax.set_zlabel("$z$ [$\\mu m$]")

    plt.title(f"Two ions, {interStr.replace('_', ' ')}")
    ax.view_init(elev=45., azim=45)
    plt.savefig(f"imgs/p8_double_movement_3d_{interStr}.svg", dpi=200)
    plt.clf()


def analytical_solution(x0, z0, v0, w0, wz2, t):
    wp = (w0 + np.sqrt(w0**2 - 2*wz2)) / 2
    wm = (w0 - np.sqrt(w0**2 - 2*wz2)) / 2

    Ap = (v0 + wm*x0) / (wm - wp)
    Am = -(v0 + wp*x0) / (wm - wp)

    x = Ap*np.cos(wp*t) + Am*np.cos(wm*t)
    y = -Ap*np.sin(wp*t) - Am*np.sin(wm*t)
    z = z0*np.cos(np.sqrt(wz2) * t)
    return np.array([x, y, z])


def plot_error():
    def plot_files(datafiles, name):
        plt.figure(tight_layout=True)
        delta_list = []
        h_list = []
        for file in datafiles:
            with open(f"problem8/{file}", "r") as datafile:
                datafile.readline()
                vals = [float(v) for v in datafile.readline().split()]
                r0 = vals[:3]
                v0 = vals[3:6]
                w0 = vals[6]
                wz2 = vals[7]
            data = np.loadtxt(
                f"problem8/{file}", skiprows=2
            ).T
            t_ray = data[0, :]
            r_ray = data[1:, :].T
            true_r = np.array([analytical_solution(
                r0[0], r0[2], v0[1], w0, wz2, t) for t in t_ray])

            h = t_ray[1]
            delta_max = np.max(np.linalg.norm(true_r - r_ray, axis=1))
            delta_list.append(delta_max)
            h_list.append(h)

            relative_error_ray = np.linalg.norm(
                (r_ray - true_r) / true_r, axis=1) * 100
            plt.plot(t_ray, relative_error_ray, label=f"h={h:5f}")
        if name == "euler":
            plt.plot([0, 50], [10, 10], "k--", label="10% error", linewidth=1)

        def sumTerm(k):
            numerator = np.log(delta_list[k]/delta_list[k-1])
            denominator = np.log(h_list[k]/h_list[k-1])
            return numerator/denominator
        r_err = (sum(sumTerm(k) for k in range(1, 4)))/3
        
        plt.xlabel("$t$ [$\\mu s$]")
        plt.ylabel("relative error [%]")
        plt.title(f"One ion, relative error with {'RK4' if name=='rk4' else 'Euler'}, $r_{{err}}={r_err:.5f}$")
        plt.legend()
        plt.yscale('log')
        plt.savefig(f"imgs/p8_err_{name}.svg")

    def sort_func(filename):
        return float(filename.split("n=")[1].split(".")[0])
    datafiles = [file for file in os.listdir('problem8') if "n=" in file]
    plot_files(
        sorted([file for file in datafiles if "euler" in file], key=sort_func), "euler")
    plot_files(
        sorted([file for file in datafiles if "rk4" in file], key=sort_func), "rk4")

funcs = [
    (plot_single_z, []), (plot_double_xy, [
        False, True]), (plot_double_phase, [False, True]),
    (plot_double_3d, [False, True]), (plot_error, [])
]

# plotting takes a while, so we use a nice progress bar
with tqdm(total=8) as pbar:
    for func, args in funcs:
        if args != []:
            for arg in args:
                func(arg)
                pbar.update(1)
        else:
            func()
            pbar.update(1)
