#pragma once

#include "../project3Utils.hpp"

// Simple particle struct, default charge and mass are from singly-charged Calcium ions
struct Particle
{
    double charge = 1;  // e
    double mass = 40.08;    // u
    Vec3 position;  // µm
    Vec3 velocity;  // µm/µs = m/s

    // create an array of N particles with normal distributed position and velocity
    // with mean 0 and std. deviation 0.1d
    template <uint N>
    static std::array<Particle, N> randomParticles(double d) {
        std::array<Particle, N> particles;
        for (size_t i = 0; i < N; i++)
        {
            particles[i] = {
                .position = arma::vec(3).randn() * 0.1 * d,
                .velocity = arma::vec(3).randn() * 0.1 * d,
            };
        }
        return particles;
    }
};
