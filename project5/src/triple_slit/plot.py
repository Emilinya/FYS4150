import numpy as np
import matplotlib.pyplot as plt

import sys
sys.path.insert(1, 'include')
from aniutils import animate
from armautils import arma_load

def get_data(n_slits):
    if n_slits == 1:
        data_dir = "src/triple_slit/output/1slit_"
    elif n_slits == 2:
        data_dir = "src/double_slit/output/"
    elif n_slits == 3:
        data_dir = "src/triple_slit/output/3slits_"
    else:
        print(f"unsuported slit count: {n_slits}")
        exit(1)

    u_rays = arma_load(f"{data_dir}us.bin", isComplex=True)
    t_ray = arma_load(f"{data_dir}ts.bin")
    V_grid = arma_load(f"{data_dir}V.bin")

    N, M2 = u_rays.shape
    M = int(np.sqrt(M2)) + 2
    assert (M-2)*(M-2) == M2

    U_grids = np.zeros((N, M, M), dtype=complex)
    for i, u_ray in enumerate(u_rays):
        U_grids[i, 1:-1, 1:-1] = u_ray.reshape(M-2, M-2)

    return U_grids, t_ray, V_grid

def plot_prob(n_slits):
    U_grids, _, _ = get_data(n_slits)
    M = U_grids.shape[1]

    final_U = U_grids[-1]

    p_detector = np.abs(final_U[:, int(0.8*(M-1))])**2
    p_detector /= np.sum(p_detector)

    plt.figure(tight_layout=True)
    plt.plot(np.linspace(0, 1, M), p_detector*100)
    plt.xlabel("y []")
    plt.ylabel("$p(y\,|\,x=0.8\,;\,t=0.002)$ [%]")
    plt.savefig(f"imgs/triple_slit/detect_{n_slits}_slits.svg")
    plt.clf()

def anim(n_slits):
    U_grids, t_ray, V_grid = get_data(n_slits)
    V_grid[np.where(V_grid == 0)] = np.nan

    animate(t_ray, np.abs(U_grids)**2, V_grid, f"imgs/triple_slit/prob_anim_{n_slits}.mp4")

anim(1)
anim(3)

plt.rcParams['font.size'] = '14'
plot_prob(1)
plot_prob(2)
plot_prob(3)
