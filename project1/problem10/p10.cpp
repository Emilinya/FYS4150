#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <chrono>

#include "../../cpp_utils/utils.hh"
#include "../problem7/p7.hh"
#include "../problem9/p9.hh"

double f(double x)
{
    return 100 * exp(-10 * x);
}

void run_general(std::vector<double> &x_vec, std::vector<double> &v_vec)
{
    size_t N = x_vec.size();
    double dx = 1. / ((double)N);
    std::vector<double> y_vec = from_func(x_vec, f);

    size_t n = N - 2;
    std::vector<double> a = filled(n - 1, -1.);
    std::vector<double> b = filled(n, 2.);
    std::vector<double> c = filled(n - 1, -1.);
    std::vector<double> g = filled(n, dx * dx);
    for (size_t i = 0; i < n; i++)
    {
        g[i] *= y_vec[i + 1];
    }
    general_algorithm(a, b, c, g, v_vec);
}

void run_special(std::vector<double> &x_vec, std::vector<double> &v_vec)
{
    size_t N = x_vec.size();
    double dx = 1. / ((double)N);
    std::vector<double> y_vec = from_func(x_vec, f);

    size_t n = N - 2;
    std::vector<double> g = filled(n, dx * dx);
    for (size_t i = 0; i < n; i++)
    {
        g[i] *= y_vec[i + 1];
    }

    special_algorithm(g, v_vec);
}

int main()
{
    std::vector<size_t> N_vals;
    for (size_t i = 1; i < 7; i++)
    {
        N_vals.push_back(pow(10, i));
    }

    std::vector<double> general_ms;
    std::vector<double> special_ms;

    size_t prewarm = 2;
    size_t iters = 10;

    Timer timer{};
    for (auto &N : N_vals)
    {
        std::cout << "N=" << N << std::endl;
        auto xn_vec = linspace(0, 1, N);
        std::vector<double> vn_vec;
        vn_vec.resize(N - 2);

        for (size_t i = 0; i < prewarm; i++)
        {
            run_general(xn_vec, vn_vec);
        }

        timer.restart();
        for (size_t i = 0; i < iters; i++)
        {
            run_general(xn_vec, vn_vec);
        }
        general_ms.push_back(timer.get_ms() / (double)iters);

        for (size_t i = 0; i < prewarm; i++)
        {
            run_special(xn_vec, vn_vec);
        }

        timer.restart();
        for (size_t i = 0; i < iters; i++)
        {
            run_special(xn_vec, vn_vec);
        }
        special_ms.push_back(timer.get_ms() / (double)iters);
    }

    std::ofstream outfile;
    outfile.open("problem10/data.dat");
    for (size_t i = 0; i < N_vals.size(); i++)
    {
        outfile << N_vals[i] << " " << general_ms[i] << " " << special_ms[i] << std::endl;
    }
    outfile.close();

    return 0;
}
