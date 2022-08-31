#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>

#include "../../cpp_utils/utils.hh"

double u(double x)
{
    return 1 - (1 - exp(-10)) * x - exp(-10 * x);
}

int main()
{
    size_t n = 1000;
    auto x_vec = linspace(0, 1, n);
    auto u_vec = from_func(x_vec, u);

    std::ofstream outfile;
    outfile.open("problem2/data.dat");
    for (size_t i = 0; i < n; i++)
    {
        outfile << x_vec[i] << " " << u_vec[i] << std::endl;
    }
    outfile.close();

    return 0;
}
