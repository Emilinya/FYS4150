#include "../../cpp_utils/utils.hpp"
#include "../problem7/integrator.hpp"
#include <memory>
#include <omp.h>

void broadScan() {
    std::vector<std::unique_ptr<PenningTrap<100>>> traps;
    traps.push_back(std::make_unique<PenningTrap<100>>(PenningTrap<100>::withRandomParticles()));
    
    // save the particle state
    std::array<Particle, 100> particles = traps[0]->particles_;

    // create data vectors
    std::vector<double> fVals = {0.1, 0.4, 0.7};
    std::vector<double> wVVals = linspace(0.2, 2.5, 800);
    std::vector<uint> trappedVals;
    trappedVals.resize(wVVals.size());

    // create 400 equal traps to enable multithreading
    for (size_t i = 1; i < wVVals.size(); i++)
    {
        traps.push_back(std::make_unique<PenningTrap<100>>(particles));
    }

    std::vector<std::unique_ptr<PenningTrapIntegrator<100>>> integrators;
    for (size_t i = 0; i < traps.size(); i++)
    {
        traps[i]->disableInteractions();
        traps[i]->wV_ = wVVals[i];
        integrators.push_back(std::make_unique<PenningTrapIntegrator<100>>(*traps[i], IntegratorType::RK4));
    }
  
    std::ofstream outfile;
    outfile.precision(14);

    Timer timer;
    for (auto &f : fVals) {
        std::cout << "f=" << f << ":" << std::endl; 
        outfile.open(string_format("problem9/data_f%g.dat", f));
        outfile << "total particles: 100\n";

        #pragma omp parallel for
        for (size_t i = 0; i < wVVals.size(); i++)
        {
            // we don't want every thread to print
            if (omp_get_thread_num() == 0) {
                std::cout << "\r  " << i+1 << "/" << wVVals.size() / omp_get_num_threads();
                fflush(stdout);
            }
 
            auto &trap = traps[i];
            auto &integrator = integrators[i];
            trap->f_ = f;
 
            integrator->integrateFast(500, 10000);
            trappedVals[i] = trap->numTrappedParticles();
            trap->particles_ = particles;
        }

        for (size_t i = 0; i < wVVals.size(); i++)
        {
            outfile << wVVals[i] << " " << trappedVals[i] << "\n";
        }
        
        outfile.close();
        std::cout << "\n  took " << timer.get_pretty() << std::endl;
        timer.restart();
    }
}

int main()
{
    broadScan();

    return 0;
}
