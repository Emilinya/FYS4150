#pragma once

#include "../project3Utils.hh"

class Particle
{
public:
    Particle(double charge, double mass, Vec3 position, Vec3 velocity)
        : charge(charge), mass(mass), position(position), velocity(velocity) {}

    double charge;  // e
    double mass;    // u
    Vec3 position;  // µm
    Vec3 velocity;  // µm/µs = m/s
};
