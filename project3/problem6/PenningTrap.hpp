#pragma once

#include <armadillo>
#include "../problem5/Particle.hpp"

template <uint N>
class PenningTrap
{
public:
    // create penning trap with no time-dependent potential term
    PenningTrap(double B0, double V0, double d, std::array<Particle, N> particles)
        : B0_(B0), V0_(V0), d_(d), f_(0), wV_(0), particles_(particles) {}

    // create penning trap with default properties and no time-dependent potential term
    PenningTrap(std::array<Particle, N> particles)
        : B0_(9.65e1), V0_(2.41e6), d_(500), f_(0), wV_(0), particles_(particles) {}

    // initialize penning trap with particles with normal distributed positions and velocities
    // uses Calsium ions and default penning trap properties
    static PenningTrap<N> withRandomParticles() {
        double d = 500;
        std::array<Particle, N> particles;
        for (size_t i = 0; i < N; i++)
        {
            particles[i] = {
                .position = arma::vec(3).randn() * 0.1 * d,
                .velocity = arma::vec(3).randn() * 0.1 * d,
            };
        }
        return PenningTrap<N>{particles};
    }

    void disableInteractions()
    {
        doInteractions_ = false;
    }
    void enableInteractions()
    {
        doInteractions_ = true;
    }

    // time-dependent perturbation of the applied potential
    inline double V(double t) {
        return V0_*(1. + f_*cos(wV_ * t));
    } 

    Vec3 getExtE(Vec3 &pos, double t)
    {
        double ratio = V(t) / (2. * d_ * d_);
        return ratio * Vec3{-2. * pos(0), -2. * pos(1), 4. * pos(2)};
    }

    inline Vec3 getExtB()
    {
        return {0, 0, B0_};
    }

    Vec3 getInteractionForce(size_t particleIdx)
    {
        Vec3 force{0, 0, 0};
        Particle particle = particles_[particleIdx];
        for (size_t i = 0; i < particles_.size(); i++)
        {
            if (i == particleIdx)
            {
                continue;
            }
            Particle other = particles_[i];
            Vec3 between = particle.position - other.position;
            double len = arma::norm(between);
            force += other.charge * between / (len * len * len);
        }
        force *= particle.charge * Units::k;
        return force;
    }

    Vec3 getExtForce(size_t particleIdx, double t)
    {
        // simplification of F = qE + qv x B
        const Particle &particle = particles_[particleIdx];
        if (arma::dot(particle.position, particle.position) > d_ * d_)
        {
            // particle is outside of trap, and is therefore not affected by
            // electric or magnetic field
            return {0, 0, 0};
        }
        double mw0 = particle.charge * B0_;
        double mwz2 = 2. * particle.charge * V(t) / (d_ * d_);
        return {
            mw0 * particle.velocity(1) + 0.5 * mwz2 * particle.position(0),
            -mw0 * particle.velocity(0) + 0.5 * mwz2 * particle.position(1),
            -mwz2 * particle.position(2),
        };
    }

    Vec3 getForce(size_t particleIdx, double t)
    {
        if (doInteractions_)
        {
            return getExtForce(particleIdx, t) + getInteractionForce(particleIdx);
        }
        else
        {
            return getExtForce(particleIdx, t);
        }
    }

    uint numTrappedParticles()
    {
        uint counter = 0;
        for (Particle &particle : particles_)
        {
            if (arma::dot(particle.position, particle.position) < d_ * d_)
            {
                counter += 1;
            }
        }
        return counter;
    }

    const double B0_;
    const double V0_;
    const double d_;
    double f_;
    double wV_;
    std::array<Particle, N> particles_;
  
private:
    double doInteractions_ = true;
};
