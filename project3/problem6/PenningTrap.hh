#pragma once

#include <armadillo>
#include "../problem5/Particle.hh"

class PenningTrap
{
public:
    PenningTrap(double B0, double V0, double d, std::vector<Particle> particles)
        : B0(B0), V0(V0), d(d), particles(particles) {}

    PenningTrap(std::vector<Particle> particles)
        : B0(9.65), V0(9.65e8), d(1e4), particles(particles) {}

    Vec3 getExternalElectricField(Vec3 pos) {
        double ratio = V0 / (2. * d * d);
        return ratio * Vec3{-2.*pos(0), -2.*pos(1), 4.*pos(2)};
    }

    inline Vec3 getExternalMagneticField() {
        return {0, 0, B0};
    }

    double getInteractionForce() {
        return 1.;
    }

    const double B0;
    double V0;
    double d;
    std::vector<Particle> particles;
};