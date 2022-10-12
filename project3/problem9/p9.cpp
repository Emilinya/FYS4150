#include "../../cpp_utils/utils.hpp"
#include "../problem7/integrator.hpp"

int main()
{
    std::vector<PenningTrap<100>> traps{PenningTrap<100>::withRandomParticles()};
    
    // save the particle state
    std::array<Particle, 100> particles = traps[0].particles_;

    std::vector<PenningTrap<100>> ogTraps;
    ogTraps.reserve(1);
    ogTraps.push_back({particles});
    ogTraps[0].disableInteractions();

    std::vector<PenningTrapIntegrator<100>> ogIntegrators;
    ogIntegrators.reserve(1);
    ogIntegrators.push_back({ogTraps[0], IntegratorType::RK4});
    ogIntegrators[0].integrateFast(2, 100);

    exit(1);

    // create data vectors
    std::vector<double> fVals = {0.1, 0.4, 0.7};
    std::vector<double> wVVals = linspace(0.2, 2.5, 400);
    std::vector<uint> trappedVals;
    trappedVals.resize(wVVals.size());

    // create 400 equal traps to enable multithreading
    traps.reserve(wVVals.size());
    for (size_t i = 1; i < wVVals.size(); i++)
    {
        traps.push_back({particles});
    }

    std::vector<PenningTrapIntegrator<100>> integrators;
    integrators.reserve(traps.size());
    for (size_t i = 0; i < traps.size(); i++)
    {
        traps[i].disableInteractions();
        traps[i].wV_ = wVVals[i];
        integrators.push_back({traps[i], IntegratorType::RK4});
    }
  
    std::ofstream outfile;
    outfile.precision(14);

    Timer timer;
    for (auto &f : fVals) {
        std::cout << "f=" << f << ":" << std::endl;
        outfile.open(string_format("problem9/data_f%g.dat", f));
        outfile << "total particles: 100\n";

        for (size_t i = 0; i < wVVals.size(); i++)
        {
            std::cout << "\r  " << i+1 << "/" << wVVals.size();
            fflush(stdout);

            auto &trap = traps[0];
            auto &integrator = integrators[0];
            trap.wV_ = wVVals[i];
            trap.f_ = f;
 
            integrator.integrateFast(500, 10000);
            trappedVals[i] = trap.numTrappedParticles();
            trap.particles_ = particles;
        }

        for (size_t i = 0; i < wVVals.size(); i++)
        {
            traps[i].particles_ = particles;
            outfile << wVVals[i] << " " << trappedVals[i] << "\n";
        }
        
        outfile.close();
        std::cout << "\n  took " << timer.get_pretty() << std::endl;
        timer.restart();
    }

    return 0;
}
