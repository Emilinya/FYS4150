#pragma once

#include <armadillo>
#include <stdexcept>

// Convert a MxM matrix into a (M-2)^2 vector.
arma::cx_vec mat2vec(arma::cx_mat U)
{
    if (!U.is_square())
    {
        throw std::invalid_argument("mat2vec: U must be a square matrix");
    }
    if (U.n_cols < 3)
    {
        throw std::invalid_argument("mat2vec: U must be 3x3 or larger");
    }

    size_t nx = U.n_cols - 2;
    size_t ny = U.n_rows - 2;
    arma::cx_vec u = arma::cx_vec(nx * ny).fill(0.);
    for (size_t i = 1; i < U.n_rows - 1; i++)
    {
        for (size_t j = 1; j < U.n_cols - 1; j++)
        {
            size_t k = (i - 1) * nx + (j - 1);
            u(k) = U(i, j);
        }
    }

    return u;
}

// Generate matrices A and B from a complex scalar 'r', and two complex vectors 'a' and 'b'.
std::pair<arma::sp_cx_mat, arma::sp_cx_mat> martix_generator(arma::cx_double r, arma::cx_vec &a, arma::cx_vec &b)
{
    size_t n = a.size();
    if (b.size() != n)
    {
        throw std::invalid_argument("martix_generator: 'a' and 'b' must have the same length");
    }
    arma::sp_cx_mat A = arma::sp_cx_mat(n, n);
    arma::sp_cx_mat B = arma::sp_cx_mat(n, n);

    size_t M = sqrt(n) + 2;
    if ((M - 2) * (M - 2) != n)
    {
        throw std::invalid_argument("martix_generator: length of 'a' must be a square number");
    }

    for (size_t i = 0; i < n; i++)
    {
        A(i, i) = a(i);
        B(i, i) = b(i);

        if (i % (M - 2) != 2 && i != n - 1)
        {
            A(i, i + 1) = -r;
            A(i + 1, i) = -r;

            B(i, i + 1) = r;
            B(i + 1, i) = r;
        }

        if (n >= (M - 2) && i < n - (M - 2))
        {
            A(i, i + (M - 2)) = -r;
            A(i + (M - 2), i) = -r;

            B(i, i + (M - 2)) = r;
            B(i + (M - 2), i) = r;
        }
    }

    return {A, B};
}

// Generates matrices A and B from a size M, step-size dt and potential matrix V.
std::pair<arma::sp_cx_mat, arma::sp_cx_mat> initialize_matricies(size_t M, double dt, arma::mat &V)
{
    if (M < 3)
    {
        throw std::invalid_argument("initialize_matricies: M must be greater than 2");
    }
    if (!V.is_square() || V.n_cols != M)
    {
        throw std::invalid_argument("initialize_matricies: V must be MxM");
    }
    double h = 1. / static_cast<double>(M - 1);

    arma::cx_double cx_i = arma::cx_double(0, 1);
    arma::cx_double r = cx_i * dt / (2 * h * h);

    size_t n = (M - 2) * (M - 2);
    arma::cx_vec a = arma::cx_vec(n).fill(0.);
    arma::cx_vec b = arma::cx_vec(n).fill(0.);

    for (size_t k = 0; k < n; k++)
    {
        size_t i = 1 + k / (M - 2);
        size_t j = 1 + k % (M - 2);

        a(k) = 1. + 4. * r + cx_i * dt * V(i, j) / 2.;
        b(k) = 1. - 4. * r - cx_i * dt * V(i, j) / 2.;
    }

    return martix_generator(r, a, b);
}
