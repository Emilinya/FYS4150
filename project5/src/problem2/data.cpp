#include "../../../cpp_utils/utils.hpp"
#include "../../include/crank_nicolson_ititializer.hpp"

int main()
{
    size_t M = 5;
    arma::mat V = arma::mat(M, M).fill(0.);
    auto [A, B] = initialize_matricies(M, 0.1, V);

    print(arma::imag(A), arma::imag(B));

    return 0;
}
