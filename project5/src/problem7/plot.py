import numpy as np
import pyarma as pa
import matplotlib.pyplot as plt

def plot_prob_diviation(name):
    us = pa.cx_mat()
    us.load(f"src/problem7/output/{name}_us.bin")
    u_rays = np.array(us)

    ts = pa.mat()
    ts.load(f"src/problem7/output/{name}_ts.bin")
    t_ray = np.array(ts).flatten()

    p_ray = np.sum(np.real(np.conj(u_rays)*u_rays), axis=1)

    plt.figure(tight_layout=True)
    plt.plot(t_ray, 1-p_ray)
    plt.xlabel("t []")
    plt.ylabel("$1-\\sum_{ij}p_{ij}$ []")
    plt.savefig(f"imgs/problem7/probdiff_{name}.svg")

plot_prob_diviation("no_wall")
plot_prob_diviation("wall")
