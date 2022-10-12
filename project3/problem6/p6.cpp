#include "PenningTrap.hpp"

int main()
{
    Particle particle{.position={1, 1, 1}, .velocity={0, 0, 0}};
    PenningTrap<1> trap({particle});
    
    std::cout << "external electric field:" << std::endl;
    std::cout << trap.getExtE(trap.particles_[0].position, 0) << std::endl;

    return 0;
}
