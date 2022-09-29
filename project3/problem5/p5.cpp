#include "Particle.hh"

int main()
{
    Particle particle(0.4, 1.1, {1, 2, 3}, {-1, -1, -1});
    
    std::cout << "pos + vel:" << std::endl;
    std::cout << particle.position + particle.velocity;

    std::cout << "k_e: " << Units::k << std::endl; 

    return 0;
}
