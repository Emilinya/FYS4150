#pragma once

#include <utility>
#include <utility>
#include <armadillo>

#include "../project3Utils.hh"
#include "../problem6/PenningTrap.hh"

enum class IntegratorType {
    RK4,
    FORWARD_EULER,
};

template <uint N>
class Integrator
{
public:
    // Numerical solver to solve differential equations on the form du/dt = f(t, u).
    // Implements the integration methods in the IntegratorType enum
    Integrator(Vec<N> (*dudt)(double, Vec<N> &), IntegratorType type)
        : dudt_(dudt), type_(type) {}

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

    void setType(IntegratorType type) {
        type_ = type;
    }


private:
    Vec<N> (*dudt_)(double, Vec<N> &);
    IntegratorType type_;

    Vec<N> step(double tn, Vec<N> &un, double h)
    {
        switch (type_)
        {
        case IntegratorType::RK4:
            return stepRK4(tn, un, h);
            break;
        case IntegratorType::FORWARD_EULER:
            return stepForwardEuler(tn, un, h);
            break;
        default:
            std::cerr << "Integrator error: unimplimented type" << std::endl;
            exit(1);
            break;
        }
    }

    Vec<N> stepRK4(double tn, Vec<N> &un, double h)
    {
        Vec<N> k1 = dudt_(tn, un);
        Vec<N> k1Step = un + 0.5 * h * k1;

        Vec<N> k2 = dudt_(tn + 0.5 * h, k1Step);
        Vec<N> k2Step = un + 0.5 * h * k2;

        Vec<N> k3 = dudt_(tn + 0.5 * h, k2Step);
        Vec<N> k3Step = un + h * k3;

        Vec<N> k4 = dudt_(tn + h, k3Step);

        return (k1 + 2. * k2 + 2. * k3 + k4) * h / 6.;
    }

    Vec<N> stepForwardEuler(double tn, Vec<N> &un, double h)
    {
        return dudt_(tn, un) * h;
    }
};

template <uint N>
class PenningTrapIntegrator
{
public:
    // Helper class which uses the Integrator class to simulate the movement of the particles in the penning trap.
    // Does not actually modify the particles in the penning trap 
    PenningTrapIntegrator(PenningTrap<N> &penningTrap, IntegratorType type) : penningTrap_(penningTrap) {
        integrator_ = Integrator<6*N>(dudt, type);
    }

    std::pair<std::vector<double>, std::vector<std::array<Particle, N>>> integrate(double tend, size_t n_steps) {
        // encode particles as one long vector
        Vec<6*N> u0;
        for (size_t i = 0; i < N; i++)
        {
            Particle &particle = penningTrap_.particles[i];
            u0(6*i) = particle.position(0);
            u0(6*i+1) = particle.position(1);
            u0(6*i+2) = particle.position(2);
            u0(6*i+3) = particle.velocity(0);
            u0(6*i+4) = particle.velocity(1);
            u0(6*i+5) = particle.velocity(2);
        }
        
        auto [tRay, uRay] = integrator_.integrate(0, tend, u0, n_steps);

        // decode particle vector into several particle states
        std::vector<std::array<Particle, N>> particleStates;
        particleStates.reserve(n_steps);

        std::array<Particle, N> particleState;

        for (size_t i = 0; i < n_steps; i++)
        {
            for (size_t j = 0; j < N; j++)
            {
                auto &u = uRay[i];
                Particle &particle = penningTrap_.particles[i];
                particleState[j] = Particle(
                    particle.charge, particle.mass,
                    {u(6*i), u(6*i+1), u(6*i+2)},
                    {u(6*i+3), u(6*i+4), u(6*i+5)}
                );
            }
            particleStates.push_back(particleState);
        }
        
        return {tRay, particleStates};
    }

    void setType(IntegratorType type) {
        integrator_.setType(type);
    }

private:
    Vec<6*N> dudt(double t, Vec<6*N> &u) {
        return t*u;
    }

    PenningTrap<N> &penningTrap_;
    Integrator<6*N> integrator_;
};
