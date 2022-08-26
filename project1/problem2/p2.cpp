#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>

std::vector<double> linspace(double a, double b, size_t n) {
    std::vector<double> vec;
    vec.reserve(n);
    for (size_t i = 0; i < n; i++)
    {
        double p = (double) i / (double) (n-1);
        vec.push_back(a + p * (b - a));
    }
    return vec;
}

double u(double x) {
    return 1 - (1 - exp(-10))*x - exp(-10*x);
}

int main(int argc, char const *argv[])
{
    size_t n = 1000;
    std::vector<double> x_vec = linspace(0, 1, n);

    std::vector<double> u_vec;
    u_vec.reserve(n);
    for (size_t i = 0; i < n; i++)
    {
        u_vec.push_back(u(x_vec[i]));
    }


    std::ofstream outfile;
    outfile.open("data.dat");
    for (size_t i = 0; i < n; i++)
    {
        outfile << x_vec[i] << " " << u_vec[i] << std::endl;
    }
    outfile.close();

    return 0;
}
