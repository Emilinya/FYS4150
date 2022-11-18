import numpy as np
import matplotlib.pyplot as plt

import sys
sys.path.insert(1, 'include')
from aniutils import animate
from armautils import arma_load

u_rays = arma_load(f"src/problem8/output/us.bin", isComplex=True)
N, M2 = u_rays.shape
M = int(np.sqrt(M2)) + 2
assert (M-2)*(M-2) == M2

U_grids = np.zeros((N, M, M), dtype=complex)
for i, u_ray in enumerate(u_rays):
    U_grids[i, 1:-1, 1:-1] = u_ray.reshape(M-2, M-2)

t_ray = arma_load(f"src/problem8/output/ts.bin")
V_grid = arma_load(f"src/problem8/output/V.bin")
V_grid[np.where(V_grid == 0)] = np.nan

animate(t_ray, np.abs(U_grids)**2, V_grid, "imgs/problem8/prob_anim.mp4")

for frac in [0, 0.5, 1]:
    idx = int(frac*(N-1))

    plt.figure(tight_layout=True)
    plt.imshow(np.real(U_grids[idx]), extent=[0, 1, 0, 1])
    plt.colorbar()
    plt.imshow(V_grid, extent=[0, 1, 0, 1], cmap="gray")
    plt.savefig(f"imgs/problem8/real_t={t_ray[idx]:.1g}.svg")
    plt.clf()

    plt.imshow(np.imag(U_grids[idx]), extent=[0, 1, 0, 1])
    plt.colorbar()
    plt.imshow(V_grid, extent=[0, 1, 0, 1], cmap="gray")
    plt.savefig(f"imgs/problem8/imag_t={t_ray[idx]:.1g}.svg")
    plt.clf()

    plt.imshow(np.abs(U_grids[idx])**2, extent=[0, 1, 0, 1])
    plt.colorbar()
    plt.imshow(V_grid, extent=[0, 1, 0, 1], cmap="gray")
    plt.savefig(f"imgs/problem8/prob_t={t_ray[idx]:.1g}.svg")
    plt.clf()
