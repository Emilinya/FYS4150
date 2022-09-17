#include "jacobiRotation.hh"
#include "../problem2/tridiag.hh"

void testJacobiEigensolver()
{
    int n = 6;
    arma::vec ana_eigenval = arma::vec(n).fill(0.);
    arma::mat ana_eigenvec = arma::mat(n, n).fill(0.);

    for (int i = 0; i < n; i++)
    {
        ana_eigenval(i) = 2 - 2 * cos((i + 1) * M_PI / static_cast<double>(n + 1));
        for (int j = 0; j < n; j++)
        {
            ana_eigenvec(i, j) = sin((j + 1) * (i + 1) * M_PI / static_cast<double>(n + 1));
        }
    }
    ana_eigenvec = arma::normalise(ana_eigenvec);

    arma::mat A = createTridiag(6, -1, 2, -1);
    JacobiResult result = jacobiEigensolver(A);

    std::cout << "eigenvalue comparison:" << std::endl;
    std::cout << "  numeric:  " << result.eigenValues.t();
    std::cout << "  analytic: " << ana_eigenval.t();
    std::cout << "eigenvector comparison:" << std::endl;
    for (int i = 0; i < n; i++)
    {
        std::cout << "  v" << i << ":" << std::endl;
        std::cout << "    numeric:  " << result.eigenVectors.col(i).t();
        std::cout << "    analytic: " << ana_eigenvec.col(i).t();
    }
}

int main()
{
    testJacobiEigensolver();
    return 0;
}
