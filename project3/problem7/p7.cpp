#include "integrator.hh"

double u(double t)
{
    // D[e^(t^2/2)] = te^(t^2/2) = tu | u(0) = 1
    return exp(t * t * 0.5);
}

Vec1 dudt(double t, Vec1 &u)
{
    return u * t;
}

int main()
{
    Integrator<1> integrator(dudt, Integrator<1>::Type::RK4);
    Vec1 u0{1};

    auto [tRay, uRay] = integrator.integrate(0, 1, {1}, 10);
    for (size_t i = 0; i < tRay.size(); i++)
    {
        std::cout << tRay[i] << ": " << uRay[i](0) << std::endl;
    }

    return 0;
}
