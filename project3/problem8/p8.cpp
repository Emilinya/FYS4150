#include "../../cpp_utils/utils.hpp"
#include "../problem7/integrator.hpp"

namespace Particles {
    Particle p1{.position = {20, 0, 20}, .velocity = {0, 25, 0}};
    Particle p2{.position = {25, 25, 0}, .velocity = {0, 40, 5}};
}

// simulate the movement of a single particle, and save the z-position as a function of time
void singleMovement()
{
    PenningTrap<1> trap{{Particles::p1}};
    PenningTrapIntegrator<1> integrator(trap, IntegratorType::RK4);
    double wz = sqrt(2. * Particles::p1.charge * trap.V0_ / (Particles::p1.mass * trap.d_ * trap.d_));

    Timer timer;
    auto pair = integrator.integrate(50, 10000);
    auto tVals = pair.first;
    auto trapStates = pair.second;
    std::cout << timer.get_pretty() << std::endl;

    std::ofstream outfile;
    outfile.precision(14);
    outfile.open("problem8/data_single_movement.dat");

    auto r0 = trapStates[0][0].position;
    auto v0 = trapStates[0][0].velocity;
    outfile << "r0x r0y r0z v0x v0y v0z wz"
            << "\n";
    outfile << r0(0) << " " << r0(1) << " " << r0(2)
            << " " << v0(0) << " " << v0(1) << " "
            << v0(2) << " " << wz << "\n";
    for (size_t i = 0; i < tVals.size(); i++)
    {
        outfile << tVals[i] << " " << trapStates[i][0].position(2) << "\n";
    }
    outfile.close();
}

// save trap states to file each line has the form "t x1 y1 z1 vx1 vy1 vz1 ... xN yN zN vxN vyN vzN"
template <uint N>
void saveTrapStates(
    std::vector<double> &times, std::vector<std::array<Particle, N>> &states, std::string filename)
{
    std::ofstream outfile;
    outfile.precision(14);
    outfile.open(filename);

    for (size_t i = 0; i < states.size(); i++)
    {
        outfile << times[i] << " ";
        for (size_t j = 0; j < N; j++)
        {
            auto &r = states[i][j].position;
            auto &v = states[i][j].velocity;
            outfile << r(0) << " " << r(1) << " " << r(2) << " ";
            outfile << v(0) << " " << v(1) << " " << v(2) << " ";
        }
        outfile << "\n";
    }
    outfile.close();
}

// simulate the movement of two particles, and save both positions and velocities as a function of time
void doubleMovement()
{
    PenningTrap<2> trap{{Particles::p1, Particles::p2}};
    PenningTrapIntegrator<2> integrator(trap, IntegratorType::RK4);

    Timer timer;
    auto [tVals, trapStates] = integrator.integrate(50, 10000);
    std::cout << timer.get_pretty() << std::endl;
    saveTrapStates<2>(tVals, trapStates, "problem8/data_double_movement_interactions.dat");

    trap.disableInteractions();
    trap.particles_ = {Particles::p1, Particles::p2};

    timer.restart();
    auto [newTVals, newTrapStates] = integrator.integrate(50, 10000);
    std::cout << timer.get_pretty() << std::endl;
    saveTrapStates<2>(newTVals, newTrapStates, "problem8/data_double_movement_no_interactions.dat");
}

// simulate movement in trap using integrator and step size h
// saves initial position and velocity, w0 and wz^2, and all positions x y z
// has a good function name
template <uint N>
void steppySavey(PenningTrapIntegrator<N> &integrator, double wz2, double w0, size_t n, std::string itStr)
{
    auto pair = integrator.integrate(50, n);
    auto tVals = pair.first;
    auto trapStates = pair.second;

    std::ofstream outfile;
    outfile.precision(14);
    outfile.open(string_format("problem8/data_single_%s_n=%d.dat", itStr.c_str(), n));

    auto r0 = trapStates[0][0].position;
    auto v0 = trapStates[0][0].velocity;
    outfile << "r0x r0y r0z v0x v0y v0z w0 wz2"
            << "\n";
    outfile << r0(0) << " " << r0(1) << " " << r0(2)
            << " " << v0(0) << " " << v0(1) << " "
            << v0(2) << " " << w0 << " " << wz2 << "\n";
    for (size_t i = 0; i < tVals.size(); i++)
    {
        auto &r = trapStates[i][0].position;
        outfile << tVals[i] << " " << r(0) << " " << r(1) << " " << r(2) << "\n";
    }
    outfile.close();
}

// simulate the movement of a single particle for multiple step sizes and each integration type
void multiStepMovement()
{
    PenningTrap<1> trap({Particles::p1});
    PenningTrapIntegrator<1> integrator(trap, IntegratorType::RK4);
    double wz2 = 2. * Particles::p1.charge * trap.V0_ / (Particles::p1.mass * trap.d_ * trap.d_);
    double w0 = Particles::p1.charge * trap.B0_ / Particles::p1.mass;

    std::vector<size_t> timeSteps{4000, 8000, 16000, 32000};
    Timer timer;
    for (auto &n : timeSteps)
    {
        steppySavey<1>(integrator, wz2, w0, n, "rk4");
        trap.particles_ = {Particles::p1};
    }
    integrator.setType(IntegratorType::FORWARD_EULER);
    for (auto &n : timeSteps)
    {
        steppySavey<1>(integrator, wz2, w0, n, "euler");
        trap.particles_ = {Particles::p1};
    }
    std::cout << timer.get_pretty() << std::endl;
}

int main()
{
    singleMovement();
    doubleMovement();
    multiStepMovement();

    return 0;
}
