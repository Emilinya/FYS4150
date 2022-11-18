#include "../../../cpp_utils/utils.hpp"
#include "../../include/schrodinger_intiializer.hpp"

int main()
{
    auto u = get_initial_state(120, {0.25, 0.05, 200}, {0.5, 0.20, 0});
    u.save("src/problem4/output/u.bin");

    auto V = get_potential(120, 1., {0.5, 0.02, 2, 0.05, 0.05});
    V.save("src/problem4/output/V.bin");

    return 0;
}
