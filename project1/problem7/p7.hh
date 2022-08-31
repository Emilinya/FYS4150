#pragma once

#include <vector>

void general_algorithm(
    std::vector<double> &a, std::vector<double> &b,
    std::vector<double> &c, std::vector<double> &g,
    std::vector<double> &v)
{
    size_t n = g.size();

    std::vector<double> b_prime;
    b_prime.reserve(n - 1);
    b_prime.push_back(b[0]);

    std::vector<double> c_prime;
    c_prime.reserve(n - 1);
    c_prime.push_back(c[0]);

    std::vector<double> g_prime;
    g_prime.reserve(n - 1);
    g_prime.push_back(g[0]);

    for (size_t i = 1; i < n-1; i++)
    {
        b_prime.push_back(b_prime[i - 1] * b[i] - a[i - 1] * c_prime[i - 1]);
        c_prime.push_back(b_prime[i - 1] * c[i]);
        g_prime.push_back(b_prime[i - 1] * g[i] - a[i - 1] * g_prime[i - 1]);
    }

    v[n - 1] = (g[n - 1] * b_prime[n - 2] - a[n - 2] * g_prime[n - 2]) / (b[n - 1] * b_prime[n - 2] - a[n - 2] * c_prime[n - 2]);

    for (size_t i = n - 1; i > 0; i--)
    {
        v[i - 1] = (g_prime[i - 1] - c_prime[i - 1] * v[i]) / b_prime[i - 1];
    }
}