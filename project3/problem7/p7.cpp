#include "../../cpp_utils/utils.hh"
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
    Integrator<1> integrator(dudt, IntegratorType::RK4);

    auto [tRay, uRayRK4] = integrator.integrate(0, 1, {1}, 10);
    integrator.setType(IntegratorType::FORWARD_EULER);
    auto [_, uRayEuler] = integrator.integrate(0, 1, {1}, 10);

    std::ofstream outfile;
    outfile.precision(14);
    outfile.open("problem7/data.dat");

    for (size_t i = 0; i < tRay.size(); i++)
    {
        outfile << tRay[i] << " " << u(tRay[i]) << " " << uRayRK4[i](0) << " " << uRayEuler[i](0) << std::endl;
    }
    outfile.close();

    return 0;
}
