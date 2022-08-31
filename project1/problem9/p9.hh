#pragma once

#include <vector>

void special_algorithm(std::vector<double> &g, std::vector<double> &v)
{
    size_t n = g.size();
    double dn = static_cast<double>(n);

    std::vector<double> g_prime;
    g_prime.reserve(n - 1);
    g_prime.push_back(g[0]);

    for (size_t i = 1; i < n-1; i++)
    {
        double di = static_cast<double>(i);
        g_prime.push_back(g_prime[i - 1] + (di + 1.) * g[i]);
    }

    v[n - 1] = (dn * g[n - 1] + g_prime[n - 2]) / (dn + 1.);
    for (size_t i = n - 1; i > 0; i--)
    {
        double di = static_cast<double>(i);
        v[i - 1] = (g_prime[i - 1] + di * v[i]) / (di + 1.);
    }
}