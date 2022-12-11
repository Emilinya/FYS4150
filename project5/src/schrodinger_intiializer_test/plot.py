import numpy as np
import matplotlib.pyplot as plt

import sys
sys.path.insert(1, 'include')
from armautils import arma_load

u = arma_load("src/schrodinger_intiializer_test/output/u.bin", isComplex=True)

plt.rcParams['font.size'] = '14'
plt.figure(tight_layout=True)
plt.imshow(np.abs(u)**2, extent=[0, 1, 0, 1])
plt.xlabel("x []")
plt.ylabel("y []")
plt.savefig("imgs/schrodinger_intiializer_test/u.svg")
plt.clf()

accu = 0
for uv in u.flat:
    accu += np.conj(uv)*uv
print("Behold: 1!", accu)

V = arma_load("src/schrodinger_intiializer_test/output/V.bin")

plt.figure(tight_layout=True)
plt.imshow(V, extent=[0, 1, 0, 1])
plt.xlabel("x []")
plt.ylabel("y []")
plt.savefig("imgs/schrodinger_intiializer_test/V.svg")
plt.clf()
