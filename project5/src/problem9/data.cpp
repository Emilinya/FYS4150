#include "../../../cpp_utils/utils.hpp"
#include "../../include/schrodinger_solver.hpp"

int main()
{
    Timer timer;
    simulate(
        "src/problem9/output/", 0.005, 2.5e-5, 0.002, 1e10,
        {0.25, 0.05, 200}, {0.5, 0.20, 0}, {0.5, 0.02, 3, 0.05, 0.05}
    );
    print(timer.get_pretty());

    return 0;
}
