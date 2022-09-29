#pragma once

#include <utility>
#include <utility>
#include <armadillo>

#include "../project3Utils.hh"

template <uint N>
class Integrator
{
public:
    enum class Type
    {
        RK4,
        FORWARD_EULER,
    };

    Integrator(Vec<N> (*f)(double, Vec<N> &), Type type)
        : f_(f), type_(type) {}

    std::pair<std::vector<double>, std::vector<Vec<N>>> integrate(double tstart, double tend, Vec<N> u0, size_t n_steps)
    {
        double h = (tend - tstart) / static_cast<double>(n_steps - 1);

        std::vector<double> tRay;
        tRay.resize(n_steps);
        tRay[0] = tstart;

        std::vector<Vec<N>> uRay;
        uRay.resize(n_steps);
        uRay[0] = u0;

        for (size_t i = 1; i < n_steps; i++)
        {
            tRay[i] = tRay[i - 1] + h;
            uRay[i] = uRay[i - 1] + step(tRay[i - 1], uRay[i - 1], h);
        }

        return {tRay, uRay};
    }

    Vec<N> step(double tn, Vec<N> &un, double h)
    {
        switch (type_)
        {
        case Type::RK4:
            return stepRK4(tn, un, h);
            break;
        case Type::FORWARD_EULER:
            return stepForwardEuler(tn, un, h);
            break;
        default:
            std::cerr << "Integrator error: unimplimented type" << std::endl;
            exit(1);
            break;
        }
    }

private:
    Vec<N> (*f_)(double, Vec<N> &);
    Type type_;

    Vec<N> stepRK4(double tn, Vec<N> &un, double h)
    {
        Vec<N> k1 = f_(tn, un);
        Vec<N> k1Step = un + 0.5 * h * k1;

        Vec<N> k2 = f_(tn + 0.5 * h, k1Step);
        Vec<N> k2Step = un + 0.5 * h * k2;

        Vec<N> k3 = f_(tn + 0.5 * h, k2Step);
        Vec<N> k3Step = un + h * k3;

        Vec<N> k4 = f_(tn + h, k3Step);

        return (k1 + 2. * k2 + 2. * k3 + k4) * h / 6.;
    }

    Vec<N> stepForwardEuler(double tn, Vec<N> &un, double h)
    {
        return f_(tn, un) * h;
    }
};
