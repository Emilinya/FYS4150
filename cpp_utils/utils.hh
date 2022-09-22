#pragma once

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <chrono>
#include <cstdio>
#include <memory>
#include <string>

std::vector<double> linspace(double a, double b, size_t n)
{
    // This function acts like np.linspace
    std::vector<double> vec;
    vec.reserve(n);
    for (size_t i = 0; i < n; i++)
    {
        double p = (double)i / (double)(n - 1);
        vec.push_back(a + p * (b - a));
    }
    return vec;
}

std::vector<double> filled(size_t n, double v)
{
    // This function creates and fills a vector
    std::vector<double> vec;
    vec.reserve(n);
    for (size_t i = 0; i < n; i++)
    {
        vec.push_back(v);
    }
    return vec;
}

std::vector<double> from_func(std::vector<double> &x_vec, double (*f)(double))
{
    // This function creates a vector the same size as x_vec, and fills it with f(x[i])
    std::vector<double> vec;
    size_t n = x_vec.size();
    vec.reserve(n);
    for (size_t i = 0; i < n; i++)
    {
        vec.push_back(f(x_vec[i]));
    }
    return vec;
}

// string formatting from https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template <typename... Args>
std::string string_format(const std::string &format, Args... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
    if (size_s <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1);
}

std::string prettify_ms(double ms) {
    if (ms > 1000.)
    {
        int s = static_cast<int>(ms / 1000.);
        if (s > 60.)
        {
            // TODO: Implement this properly
            int m = static_cast<int>(s / 60.);
            ms -= 1000. * s;
            s -= m * 60;
            return string_format("%d m %d s %.0f ms", m, s, ms);
        }
        else
        {
            ms -= 1000. * s;
            if (ms == 0)
            {
                return string_format("%d s", s);
            }
            else
            {
                return string_format("%d s %.0f ms", s, ms);
            }
        }
    }
    else
    {
        return string_format("%.3g ms", ms);
    }
}

class Timer
{
    // This is a timer class to simplify timing (especially removing all those long type declarations)
public:
    Timer();
    double get_ms();
    std::string get_pretty();
    void restart();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

Timer::Timer()
{
    start = std::chrono::high_resolution_clock::now();
}

double Timer::get_ms()
{
    // Get elapsed time in ms as a double
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    return duration.count();
}

std::string Timer::get_pretty()
{
    // Get elapsed time as a nice, formatted string
    return prettify_ms(Timer::get_ms());
}

void Timer::restart()
{
    start = std::chrono::high_resolution_clock::now();
}
