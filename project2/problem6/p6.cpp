#include "../../cpp_utils/utils.hh"
#include "../problem4/jacobiRotation.hh"
#include "../problem2/tridiag.hh"

void saveDiscretization(size_t n)
{
    double h = 1. / static_cast<double>(n);
    arma::mat A = createTridiag(n - 1, -1. / h, 2. / h, -1. / h);
    JacobiResult result = jacobiEigensolver(A, 1e-8, 100000);
    assert(result.didConverge);

    std::ofstream outfile;
    outfile.precision(14);
    outfile.open(string_format("problem6/data_%d.dat", n));
    for (size_t i = 0; i < 3; i++)
    {
        outfile << result.eigenValues(i);
        for (size_t j = 0; j < n-1; j++) {
            outfile << " " << result.eigenVectors(j, i);
        }
        outfile << std::endl;
    }
    outfile.close();
}

int main()
{
    saveDiscretization(10);
    saveDiscretization(100);

    return 0;
}
