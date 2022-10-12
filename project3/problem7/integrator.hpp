#pragma once

#include <utility>
#include <utility>
#include <armadillo>

#include "../project3Utils.hpp"
#include "../problem6/PenningTrap.hpp"

enum class IntegratorType
{
    RK4,
    FORWARD_EULER,
};

template <uint N>
class Integrator
{
public:
    // Numerical solver to solve differential equations on the form du/dt = f(t, u).
    // Implements the integration methods in the IntegratorType enum
    Integrator(std::function<Vec<N>(double, Vec<N> &)> dudt, IntegratorType type)
        : dudt_(dudt), type_(type) {}

    std::pair<std::vector<double>, std::vector<Vec<N>>>
    integrate(double tstart, double tend, Vec<N> u0, size_t n_steps)
    {
        double h = (tend - tstart) / static_cast<double>(n_steps);

        std::vector<double> tRay;
        tRay.resize(n_steps+1);
        tRay[0] = tstart;

        std::vector<Vec<N>> uRay;
        uRay.resize(n_steps+1);
        uRay[0] = u0;

        for (size_t i = 1; i < n_steps+1; i++)
        {
            tRay[i] = tRay[i - 1] + h;
            uRay[i] = uRay[i - 1] + step(tRay[i - 1], uRay[i - 1], h);
        }

        return {tRay, uRay};
    }

    std::pair<double, Vec<N>> integrateFast(double tstart, double tend, Vec<N> u0, size_t n_steps)
    {
        double h = (tend - tstart) / static_cast<double>(n_steps);

        double t = tstart;
        Vec<N> u = u0;

        for (size_t i = 1; i < n_steps+1; i++)
        {
            u = u + step(t, u, h);
            t = t + h;
        }

        return {t, u};
    }

    void setType(IntegratorType type)
    {
        type_ = type;
    }

private:
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

    std::function<Vec<N>(double, Vec<N> &)> dudt_;
    IntegratorType type_;
};

template <uint N>
class PenningTrapIntegrator
{
public:
    // Helper class which uses the Integrator class to simulate the movement of the particles in the penning trap.
    // Modifies the particles in the penning trap
    PenningTrapIntegrator(PenningTrap<N> &penningTrap, IntegratorType type)
        : penningTrap_(penningTrap)
        , integrator_([this] (double t, Vec<6 * N> &u) { return this->dudtFunc(t, u); }, type) {}

    std::pair<std::vector<double>, std::vector<std::array<Particle, N>>>
    integrate(double tend, size_t n_steps)
    {
        // encode particles as one long vector
        Vec<6 * N> u0;
        for (size_t i = 0; i < N; i++)
        {
            Particle &particle = penningTrap_.particles_[i];
            u0(6 * i) = particle.position(0);
            u0(6 * i + 1) = particle.position(1);
            u0(6 * i + 2) = particle.position(2);
            u0(6 * i + 3) = particle.velocity(0);
            u0(6 * i + 4) = particle.velocity(1);
            u0(6 * i + 5) = particle.velocity(2);
        }

        auto [tRay, uRay] = integrator_.integrate(0, tend, u0, n_steps);

        // decode particle vector into several particle states
        std::vector<std::array<Particle, N>> particleStates;
        particleStates.reserve(n_steps+1);

        std::array<Particle, N> particleState;
        for (size_t i = 0; i < n_steps+1; i++)
        {
            for (size_t j = 0; j < N; j++)
            {
                auto &u = uRay[i];
                Particle &particle = penningTrap_.particles_[j];
                particleState[j] = {
                    particle.charge, particle.mass,
                    {u(6 * j), u(6 * j + 1), u(6 * j + 2)},
                    {u(6 * j + 3), u(6 * j + 4), u(6 * j + 5)}
                };
            }
            particleStates.push_back(particleState);
        }

        // update trap to give particles correct positions
        penningTrap_.particles_ = particleStates[particleStates.size() - 1];

        return {tRay, particleStates};
    }

    void integrateFast(double tend, size_t n_steps)
    {
        // encode particles as one long vector
        Vec<6 * N> u0;
        for (size_t i = 0; i < N; i++)
        {
            Particle &particle = penningTrap_.particles_[i];
            u0(6 * i) = particle.position(0);
            u0(6 * i + 1) = particle.position(1);
            u0(6 * i + 2) = particle.position(2);
            u0(6 * i + 3) = particle.velocity(0);
            u0(6 * i + 4) = particle.velocity(1);
            u0(6 * i + 5) = particle.velocity(2);
        }

        auto [t, u] = integrator_.integrateFast(0, tend, u0, n_steps);

        // update trap to give particles correct positions
        for (size_t i = 0; i < N; i++)
        {
            Particle &particle = penningTrap_.particles_[i];
            particle.position(0) = u(6 * i);
            particle.position(1) = u(6 * i + 1);
            particle.position(2) = u(6 * i + 2);
            particle.velocity(0) = u(6 * i + 3);
            particle.velocity(1) = u(6 * i + 4);
            particle.velocity(2) = u(6 * i + 5);
        }
    }

    void setType(IntegratorType type)
    {
        integrator_.setType(type);
    }

private:
    Vec<6 * N> dudtFunc(double t, Vec<6 * N> &u) {
        for (size_t i = 0; i < N; i++)
        {
            // update positions of particles in penningTrap
            Particle &particle = penningTrap_.particles_[i];
            particle.position(0) = u(6 * i);
            particle.position(1) = u(6 * i + 1);
            particle.position(2) = u(6 * i + 2);
            particle.velocity(0) = u(6 * i + 3);
            particle.velocity(1) = u(6 * i + 4);
            particle.velocity(2) = u(6 * i + 5);
        }

        Vec<6 * N> dudt;
        for (size_t i = 0; i < N; i++)
        {
            // dx/dt = v
            const Particle &particle = penningTrap_.particles_[i];
            dudt(6 * i) = particle.velocity(0);
            dudt(6 * i + 1) = particle.velocity(1);
            dudt(6 * i + 2) = particle.velocity(2);

            // dv/dt = a
            Vec3 acc = penningTrap_.getForce(i, t) / particle.mass;
            dudt(6 * i + 3) = acc(0);
            dudt(6 * i + 4) = acc(1);
            dudt(6 * i + 5) = acc(2);
        }

        return dudt;
    }

    PenningTrap<N> &penningTrap_;
    Integrator<6 * N> integrator_;
};
