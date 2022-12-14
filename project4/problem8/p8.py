from __future__ import annotations

import numpy as np
import matplotlib.pyplot as plt
from dataclasses import dataclass


@dataclass
class Quantity:
    mean: np.ndarray
    std: np.ndarray

    # iter and next are needed to enable use of the explosion operator
    def __iter__(self):
        self.i = 0
        return self

    def __next__(self):
        self.i += 1
        if self.i == 1:
            return self.mean
        elif self.i == 2:
            return self.std
        else:
            raise StopIteration

    @classmethod
    def from_grid(cls, grid: np.ndarray):
        mean = np.mean(grid, axis=0)
        std = np.std(grid, axis=0)

        return cls(mean, std)


@dataclass
class Data:
    L: int
    Ts: np.ndarray
    eps: Quantity
    absm: Quantity
    Cv: Quantity
    X: Quantity


def plot_w_std(xs: np.ndarray, quantity: Quantity, label: str, color, alpha=0.4):
    plt.fill_between(
        xs, quantity.mean - quantity.std, quantity.mean + quantity.std,
        color=color, alpha=alpha, edgecolor=color
    )
    plt.plot(xs, quantity.mean, ".-", color=color, label=label, markersize=4)


def get_datas(Ls: list[int]) -> list[Data]:
    datas = []
    for L in Ls:
        with open(f"problem8/data-L{L}.dat", "r") as datafile:
            repeats, nTs = [
                int(pair.split("=")[1]) for pair in
                datafile.readline().split(", ")
            ]
            eps_grid = np.zeros((repeats, nTs))
            absm_grid = np.zeros_like(eps_grid)
            Cv_grid = np.zeros_like(eps_grid)
            X_grid = np.zeros_like(eps_grid)
            T_ray = np.zeros(nTs)
            for i in range(repeats):
                T_ray, eps_ray, absm_ray, Cv_ray, X_ray = np.loadtxt(
                    datafile, skiprows=1, max_rows=nTs
                ).T
                eps_grid[i, :] = eps_ray
                absm_grid[i, :] = absm_ray
                Cv_grid[i, :] = Cv_ray
                X_grid[i, :] = X_ray

            datas.append(Data(
                L, T_ray,
                Quantity.from_grid(eps_grid),
                Quantity.from_grid(absm_grid),
                Quantity.from_grid(Cv_grid),
                Quantity.from_grid(X_grid),
            ))

    return datas


def get_critical_Ts(datas: list[Data]):
    def refine_topT(Ts, Cvs, topT_guess, top_points):
        r = int((top_points-1)/2)
        top_idx = np.argmax(Ts > topT_guess)

        c, b, _ = np.polyfit(Ts[top_idx-r:top_idx+r], Cvs[top_idx-r:top_idx+r], 2)

        # analytical maximum of the polynomial approximation
        return -b/(2*c)


    plt.figure(tight_layout=True)
    colors = plt.rcParams['axes.prop_cycle'].by_key()['color']

    critical_temps = []
    critical_Cvs = []
    top_Ts_list = []
    poly_approxs = []

    for data, color in zip(datas, colors):
        # I tried several r values, 11 is the one that results in the best Tc(L=???)
        r = 11

        # initial guess: teperature with largest Cv
        critical_T = data.Ts[np.argmax(data.Cv.mean)]

        # refinement: top of a polynomial approximation
        critical_T = refine_topT(data.Ts, data.Cv.mean, critical_T, 2*r + 1)

        # perfection: top of new polynomial approximation
        top_idx = np.argmax(data.Ts > critical_T)
        top_Ts = data.Ts[top_idx-r:top_idx+r]
        top_Ts_list.append(top_Ts)

        c, b, a = np.polyfit(top_Ts, data.Cv.mean[top_idx-r:top_idx+r], 2)
        critical_T = -b/(2*c)

        critical_temps.append(critical_T)
        critical_Cvs.append(a + b*critical_T + c*critical_T**2)

        poly_approx = a + b*top_Ts + c*top_Ts**2
        poly_approxs.append(poly_approx)

        plot_w_std(data.Ts, data.Cv, label=f"L={data.L}", color=color, alpha=0.1)

    for critical_temp, critical_Cv, color in zip(critical_temps, critical_Cvs, colors):
        plt.axvline(critical_temp, ymax=(
            critical_Cv - 0.9)/(2.5 - 0.9), color=color)
    for top_Ts, poly_approx in zip(top_Ts_list, poly_approxs):
        plt.plot(top_Ts, poly_approx, "k-")

    plt.xlim(2.15, 2.4)
    plt.ylim(0.9, 2.5)
    plt.ylabel(f"$C_V$ [$k_B$]")
    plt.xlabel("T [$J/k_B$]")
    plt.legend()
    plt.savefig(f"imgs/p8_Cv.svg")

    return critical_temps


def plot_Ls(datas: list[Data], critical_Ts):
    colors = plt.rcParams['axes.prop_cycle'].by_key()['color']
    for data, critical_T in zip(datas, critical_Ts):
        plt.figure(tight_layout=True)
        plt.axvline(critical_T, color="k",
                    linestyle="--", label="critical $T$")
        plot_w_std(data.Ts, data.eps, "$\\left<\\epsilon\\right>$", colors[0])
        plot_w_std(data.Ts, data.absm, "$\\left<|m|\\right>$", colors[1])
        plot_w_std(data.Ts, data.Cv, "$C_V$", colors[2])

        plt.ylabel("quantities []")
        plt.xlabel("T [$J/k_B$]")
        plt.ylim(-1.88, 3)
        plt.legend()
        plt.savefig(f"imgs/p8_L={data.L}.svg")

        plot_w_std(data.Ts, data.X, "$\\chi$", colors[3])
        plt.legend()
        plt.ylim(-1.88, np.max(data.X.mean)*1.05)
        plt.savefig(f"imgs/p8_L={data.L}_X.svg")


def getInfinicrit(L_ray, critical_T_ray):
    (infinicrit, a), covariance = np.polyfit(L_ray, L_ray*critical_T_ray, 1, cov=True)
    linfit_err, _ = np.sqrt(np.diag(covariance))

    plt.figure(tight_layout=True)
    plt.plot(L_ray, a + infinicrit*L_ray, "k--", label="linfit")
    plt.plot(L_ray, L_ray*critical_T_ray, "o", label="data")
    plt.xlabel("L []")
    plt.ylabel(f"$LT_c(L)$ [$J/k_B$]")

    plt.legend()
    plt.savefig(f"imgs/p8_infinicrit.svg")

    return infinicrit, linfit_err

if __name__ == "__main__":
    plt.rcParams['font.size'] = '14'

    Ls = [40, 60, 80, 100]
    datas = get_datas(Ls)

    critical_Ts = get_critical_Ts(datas)
    plot_Ls(datas, critical_Ts)

    infinicrit, linfit_err = getInfinicrit(np.array(Ls), np.array(critical_Ts))

    true_infinicrit = 2/np.log(1 + np.sqrt(2))
    print(f"Tc(L=???) = {infinicrit:.5f} ?? {linfit_err:.5f}")
    print(f"Tc(L=???) = {true_infinicrit}")
    print(f"abs err = {abs(infinicrit - true_infinicrit):.5f}")
    print(f"rel err = {abs(infinicrit - true_infinicrit)/true_infinicrit*100:.5f} %")
