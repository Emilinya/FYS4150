import numpy as np
import matplotlib.pyplot as plt


def display_poly(a, b, c):
    # function to avoid + -x in my plot label >:(
    string = f"{a:.3f}n^2"
    string += " + " if b > 0 else " - "
    string += f"{abs(b):.3f}n"
    string += " + " if c > 0 else " - "
    string += f"{abs(c):.3f}"
    return string


def plot_profile(type):
    n_ray, iters_ray = np.loadtxt(f"problem5/data_{type}.dat").T
    a, b, c = np.polyfit(n_ray, iters_ray, 2)

    plt.figure(constrained_layout=True)
    plt.plot(n_ray, iters_ray, ".-", label=f"profile of {type} matrix")
    # add rounding to a, b, and c so the function actually matches the label
    plt.plot(n_ray, round(a, 3)*n_ray**2 + round(b, 3)*n_ray +
             round(c, 3), "--k", label=f"f(n) = ${display_poly(a, b, c)}$")
    plt.xlabel("n []")
    plt.ylabel("iterations []")
    plt.legend()
    plt.savefig(f"p5_{type}.png", dpi=200)


plot_profile("sparce")
plot_profile("dense")
