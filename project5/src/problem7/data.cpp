#include "../../../cpp_utils/utils.hpp"
#include "../../include/schrodinger_solver.hpp"

int main()
{
    Timer timer;
    simulate(
        "src/problem7/output/no_wall", 0.005, 2.5e-5, 0.008, 0,
        {0.25, 0.05, 200}, {0.5, 0.05, 0}, {0.5, 0.02, 2, 0.05, 0.05}
    );
    print(timer.get_pretty()); 

    timer.restart();
    simulate(
        "src/problem7/output/wall", 0.005, 2.5e-5, 0.008, 1e10,
        {0.25, 0.05, 200}, {0.5, 0.10, 0}, {0.5, 0.02, 2, 0.05, 0.05}
    );
    print(timer.get_pretty()); 

    return 0;
}
