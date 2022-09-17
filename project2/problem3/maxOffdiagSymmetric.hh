#pragma once

#include <armadillo>
#include <assert.h>

struct MatElement
{
    size_t i;
    size_t j;
    double val;
};

MatElement maxOffdiagSymmetric(const arma::mat &A)
{
    assert(A.is_symmetric());
    size_t n = A.n_rows;

    double max = A(1, 0);
    size_t maxi = 1;
    size_t maxj = 0;

    for (size_t j = 0; j < n; j++)
    {
        for (size_t i = j; i < n; i++)
        {
            if ((i != j) && (fabs(A(i, j)) > fabs(max)))
            {
                max = A(i, j);
                maxi = i;
                maxj = j;
            }
        }
    }

    return {maxi, maxj, max};
}