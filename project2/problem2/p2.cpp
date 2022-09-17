#include "../../cpp_utils/utils.hh"
#include "tridiag.hh"

void testTridiag()
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
    arma::vec num_eigenval = arma::vec(n).fill(0.);
    arma::mat num_eigenvec = arma::mat(n, n).fill(0.);
    arma::eig_sym(num_eigenval, num_eigenvec, A);

    std::cout << "eigenvalue comparison:" << std::endl;
    std::cout << "  numeric:  " << num_eigenval.t();
    std::cout << "  analytic: " << ana_eigenval.t();
    std::cout << "eigenvector comparison:" << std::endl;
    for (int i = 0; i < n; i++)
    {
        std::cout << "  v" << i << ":" << std::endl;
        std::cout << "    numeric:  " << num_eigenvec.col(i).t();
        std::cout << "    analytic: " << ana_eigenvec.col(i).t();
    }
}

int main()
{
    testTridiag();
    return 0;
}
