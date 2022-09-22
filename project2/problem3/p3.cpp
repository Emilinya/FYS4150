#include "../../cpp_utils/utils.hh"
#include "../problem2/tridiag.hh"
#include "maxOffdiagSymmetric.hh"

void testMaxOffdiagSymmetric()
{
    arma::mat A{{1, 0, 0, 0.5}, {0, 1, -0.7, 0}, {0, -0.7, 1, 0}, {0.5, 0, 0, 1}};
    auto maxElem = maxOffdiagSymmetric(A);

    std::cout << "testing maxOffdiagSymmetric with matrix:" << std::endl;
    std::cout << A;
    std::cout << "got max=" << maxElem.val << " at position (" << maxElem.i << ", " << maxElem.j << "), correct max is -0.7" << std::endl;
}

int main()
{
    testMaxOffdiagSymmetric();
    return 0;
}
