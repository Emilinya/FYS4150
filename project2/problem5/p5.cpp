#include "../problem4/jacobiRotation.hh"
#include "../problem2/tridiag.hh"

void profileJacobiEigensolverSparse()
{
    std::ofstream outfile;
    outfile.open("problem5/data_sparce.dat");
    outfile.precision(14);
    for (size_t n = 3; n < 100; n++)
    {
        std::cout << "\r" << n + 1 << "/" << 100;
        std::fflush(stdout);
        // a tridiagonal matrix is sparce
        arma::mat A = createTridiag(n, -1, 2, -1);
        JacobiResult result = jacobiEigensolver(A, 1e-8, 100000);
        outfile << n << " " << result.iterations << std::endl;
    }
    std::cout << std::endl;
    outfile.close();
}

void profileJacobiEigensolverDense()
{
    std::ofstream outfile;
    outfile.open("problem5/data_dense.dat");
    outfile.precision(14);
    for (size_t n = 3; n < 100; n++)
    {
        std::cout << "\r" << n + 1 << "/" << 100;
        std::fflush(stdout);
        // create dense symmetric matrix
        arma::mat A = arma::mat(n, n).randn();
        A = arma::symmatu(A);
        JacobiResult result = jacobiEigensolver(A, 1e-8, 100000);
        outfile << n << " " << result.iterations << std::endl;
    }
    std::cout << std::endl;
    outfile.close();
}

int main()
{
    profileJacobiEigensolverSparse();
    profileJacobiEigensolverDense();
    return 0;
}
