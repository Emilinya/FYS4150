#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <chrono>

#include "../../cpp_utils/utils.hh"
#include "p9.hh"

double f(double x)
{
    return 100*exp(-10*x);
}

void use_special_algorithm(std::vector<double> &x_vec, std::vector<double> &v_vec)
{
    Timer timer{};
    
    size_t N = x_vec.size();
    double dx = 1./((double) N);
    std::vector<double> y_vec = from_func(x_vec, f);

    size_t n = N - 2;
    std::vector<double> g = filled(n, dx*dx);
    for (size_t i = 0; i < n; i++) {
        g[i] *= y_vec[i+1];
    }

    std::cout << "  creating vecors: " << timer.get_pretty() << std::endl;

    timer.restart();
    special_algorithm(g, v_vec);
    std::cout << "  running the algorithm: " << timer.get_pretty() << std::endl;
}

int main()
{
    std::ofstream outfile;
    outfile.open("problem9/data.dat");

    std::vector<size_t> n_vec{10};
    size_t n_vals = 5;
    for (size_t i = 0; i < n_vals-1; i++)
    {
        n_vec.push_back(10*n_vec[i]);
    }
    
    for (auto &n: n_vec) {
        n = n+2;
        std::cout << "n=" << n << std::endl;

        auto xn_vec = linspace(0, 1, n);
        std::vector<double> vn_vec;
        vn_vec.resize(n-2);

        use_special_algorithm(xn_vec, vn_vec);

        Timer timer{};
        for (size_t i = 1; i < n-1; i++)
        {
            outfile << xn_vec[i] << " " << vn_vec[i-1] << std::endl;
        }
        outfile << " --- " << std::endl;
        std::cout << "  saving to file: " << timer.get_pretty() << std::endl;
    }
    outfile.close();

    return 0;
}
