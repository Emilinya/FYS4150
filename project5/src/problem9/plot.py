import numpy as np
import matplotlib.pyplot as plt

import sys
sys.path.insert(1, 'include')
from aniutils import animate
from armautils import arma_load

def plot_prob(n_slits):
    if n_slits == 2:
        data_dir = "src/problem8/output"
    elif n_slits == 3:
        data_dir = "src/problem9/output"
    else:
        print(f"unsuported slit count: {n_slits}")
        return

    u_rays = arma_load(f"{data_dir}/us.bin", isComplex=True)
    N, M2 = u_rays.shape
    M = int(np.sqrt(M2)) + 2
    assert (M-2)*(M-2) == M2

    U_grids = np.zeros((N, M, M), dtype=complex)
    for i, u_ray in enumerate(u_rays):
        U_grids[i, 1:-1, 1:-1] = u_ray.reshape(M-2, M-2)

    final_U = U_grids[-1]

    p_detector = np.abs(final_U[:, int(0.8*(M-1))])**2
    p_detector /= np.sum(p_detector)

    plt.figure(tight_layout=True)
    plt.plot(np.linspace(0, 1, M), p_detector)
    plt.xlabel("y []")
    plt.ylabel("$p(y\,|\,x=0.8\,;\,t=0.002)$")
    plt.savefig(f"imgs/problem9/detect_{n_slits}_slits.svg")
    plt.clf()

plot_prob(2)
plot_prob(3)

# animate 3 slits

u_rays = arma_load(f"src/problem9/output/us.bin", isComplex=True)
N, M2 = u_rays.shape
M = int(np.sqrt(M2)) + 2
assert (M-2)*(M-2) == M2

U_grids = np.zeros((N, M, M), dtype=complex)
for i, u_ray in enumerate(u_rays):
    U_grids[i, 1:-1, 1:-1] = u_ray.reshape(M-2, M-2)

t_ray = arma_load(f"src/problem9/output/ts.bin")
V_grid = arma_load(f"src/problem9/output/V.bin")
V_grid[np.where(V_grid == 0)] = np.nan

animate(t_ray, np.abs(U_grids)**2, V_grid, "imgs/problem9/prob_anim.mp4")
