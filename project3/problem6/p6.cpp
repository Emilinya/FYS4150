#include "PenningTrap.hh"

int main()
{
    PenningTrap<0> trap({});
    
    std::cout << "external electric field:" << std::endl;
    std::cout << trap.getExternalElectricField({1, 1, 1}) << std::endl;

    return 0;
}
