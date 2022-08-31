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
    std::vector<double> vec;
    vec.reserve(n);
    for (size_t i = 0; i < n; i++) {
        double p = (double)i / (double)(n - 1);
        vec.push_back(a + p*(b - a));
    }
    return vec;
}

std::vector<double> filled(size_t n, double v)
{
    std::vector<double> vec;
    vec.reserve(n);
    for (size_t i = 0; i < n; i++) {
        vec.push_back(v);
    }
    return vec;
}

std::vector<double> from_func(std::vector<double> &x, double (*f)(double))
{
    std::vector<double> vec;
    size_t n = x.size();
    vec.reserve(n);
    for (size_t i = 0; i < n; i++)
    {
        vec.push_back(f(x[i]));
    }
    return vec;
}

// string formatting from https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1;
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 );
}

class Timer
{
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

double Timer::get_ms() {
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    return duration.count();
}

std::string Timer::get_pretty() {
    double ms = Timer::get_ms();

    if (ms > 1000.) {
        int s = static_cast<int>(ms / 1000.);
        if (s > 60.) {
            int m = static_cast<int>(s / 60.);
            s -= m*60;
            ms -= 1000.*s;
            return string_format("%d m %d s %.0f ms", m, s, ms);
        } else {
            ms -= 1000.*s;
            if (ms == 0) {
                return string_format("%d s", s);
            } else {
               return string_format("%d s %.0f ms", s, ms);
            }
        }
    } else {
        return string_format("%.3g ms", ms);
    }
}

void Timer::restart() {
    start = std::chrono::high_resolution_clock::now();
}

