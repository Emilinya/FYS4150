#pragma once

#include <armadillo>
#include <assert.h>
#include "../problem3/maxOffdiagSymmetric.hh"

struct JacobiResult
{
    arma::vec eigenValues;
    arma::mat eigenVectors;
    int iterations;
    bool didConverge;
};

void jacobiRotate(arma::mat &A, arma::mat &R, const int k, const int l)
{
    int n = A.n_cols;
    double tau = (A(l, l) - A(k, k)) / (2. * A(k, l));
    double t; // tan
    // choose t to be as small as possible
    if (tau > 0) {
        t = -tau + sqrt(1 + tau * tau);
    } else {
        t = -tau - sqrt(1 + tau * tau);
    }

    double c = 1 / (sqrt(1 + t * t)); // cos
    double s = c * t;                 // sin

    arma::vec Acolk = A.col(k);
    arma::vec Acoll = A.col(l);
    arma::vec Rcolk = R.col(k);
    arma::vec Rcoll = R.col(l);

    A(k, k) = Acolk(k) * c * c - 2 * Acoll(k) * c * s + Acoll(l) * s * s;
    A(l, l) = Acoll(l) * c * c + 2 * Acoll(k) * c * s + Acolk(k) * s * s;
    A(k, l) = 0, A(l, k) = 0;

    for (int i = 0; i < n; i++)
    {
        R(i, k) = Rcolk(i) * c - Rcoll(i) * s;
        R(i, l) = Rcoll(i) * c + Rcolk(i) * s;
        if (i == k || i == l)
        {
            continue;
        }
        A(i, k) = Acolk(i) * c - Acoll(i) * s, A(k, i) = A(i, k);
        A(i, l) = Acoll(i) * c + Acolk(i) * s, A(l, i) = A(i, l);
    }
}

JacobiResult jacobiEigensolver(const arma::mat &A, const double epsilon = 1e-8, const int maxiter = 1000)
{
    assert(A.is_symmetric());
    size_t n = A.n_cols;
    int iterations = 0;
    bool didConverge = false;

    MatElement maxElem = maxOffdiagSymmetric(A);

    arma::mat Acopy(A);
    arma::mat R = arma::eye(n, n);

    for (; iterations < maxiter; iterations++)
    {
        if (fabs(maxElem.val) < epsilon)
        {
            didConverge = true;
            break;
        }
        jacobiRotate(Acopy, R, maxElem.i, maxElem.j);
        maxElem = maxOffdiagSymmetric(Acopy);
    }
    R = arma::normalise(R);

    // we have our eigenValues and eigenVectors, but we want to sort
    // them so the eigenValues are in increasing order
    arma::vec eigenVals = Acopy.diag();
    arma::uvec sortIdx = arma::sort_index(eigenVals);
    arma::uvec range = arma::regspace<arma::uvec>(0, n - 1);

    return {eigenVals(sortIdx), R(range, sortIdx), iterations, didConverge};
}