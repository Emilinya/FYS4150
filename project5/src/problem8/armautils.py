import numpy as np
import pyarma as pa

def arma_load(filename, isComplex=False):
    if isComplex:
        mat = pa.cx_mat()
    else:
        mat = pa.mat()
    mat.load(filename)

    array = np.array(mat)
    if array.shape[1] == 1:
        array = array.flatten()

    return array
