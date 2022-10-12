#pragma once

#include "../project3Utils.hpp"

// Simple particle struct, default charge and mass are from Calcium ions
struct Particle
{
    double charge = 2;  // e
    double mass = 40.08;    // u
    Vec3 position;  // µm
    Vec3 velocity;  // µm/µs = m/s
};
