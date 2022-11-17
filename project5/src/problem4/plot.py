import numpy as np
import pyarma as pa
import matplotlib.pyplot as plt

u = pa.cx_mat()
u.load("src/problem4/output/u.bin")
u = np.array(u)

plt.imshow(np.abs(u))
plt.savefig("imgs/problem4/u.svg")
plt.clf()

accu = 0
for uv in u.flat:
    accu += np.conj(uv)*uv
print("Behold: 1!", accu)

V = pa.mat()
V.load("src/problem4/output/V.bin")

plt.imshow(V)
plt.savefig("imgs/problem4/V.svg")
plt.clf()
