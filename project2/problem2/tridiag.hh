#pragma once

#include <armadillo>

arma::mat createTridiag(int n, double a, double b, double c)
{
    arma::mat A = arma::mat(n, n).fill(0.);
    A(0, 0) = b;
    for (int i = 1; i < n; i++)
    {
        A(i, i - 1) = a;
        A(i, i) = b;
        A(i - 1, i) = c;
    }

    return A;
}