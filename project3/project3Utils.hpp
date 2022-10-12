#pragma once

#include <armadillo>

template <uint N>
using Vec = arma::dvec::fixed<N>;

typedef Vec<1> Vec1;
typedef Vec<2> Vec2;
typedef Vec<3> Vec3;

namespace Units {
    double k = 1.38935333e5;
    double T = 9.64852558;
    double V = 9.64852558e7;
};
