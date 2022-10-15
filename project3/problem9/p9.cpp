#include "../../cpp_utils/utils.hpp"
#include "../problem7/integrator.hpp"
#include <memory>
#include <omp.h>

typedef std::vector<std::unique_ptr<PenningTrap<100>>> TrapVec;
typedef std::vector<std::unique_ptr<PenningTrapIntegrator<100>>> IntVec;

// we need to create multiple identical traps and integrators to enable multithreading
void createTraps(
    std::array<Particle, 100> &particles, std::vector<double> &wVVals,
    TrapVec &traps, IntVec &integrators)
{
    // we create an amount of traps equal ot the number of wV values
    // we only need to crate an amount equal to the number of threads,
    // but doing it this way is simpler
    for (size_t i = 0; i < wVVals.size(); i++)
    {
        traps.push_back(std::make_unique<PenningTrap<100>>(particles));
        traps[i]->disableInteractions();
        traps[i]->wV_ = wVVals[i];

        integrators.push_back(std::make_unique<PenningTrapIntegrator<100>>(*traps[i], IntegratorType::RK4));
    }
}

std::vector<uint> parallelInt(
    std::array<Particle, 100> &particles,
    std::vector<std::unique_ptr<PenningTrap<100>>> &traps,
    std::vector<std::unique_ptr<PenningTrapIntegrator<100>>> &integrators,
    double f, std::vector<double> &wVVals)
{
    size_t n = wVVals.size();
    std::vector<uint> trappedVals;
    trappedVals.resize(n);

    #pragma omp parallel for
    for (size_t i = 0; i < n; i++)
    {
        // we don't want every thread to print
        if (omp_get_thread_num() == 0)
        {
            std::cout << "\r  " << i + 1 << "/" << n / omp_get_num_threads();
            fflush(stdout);
        }

        auto &trap = traps[i];
        auto &integrator = integrators[i];

        trap->f_ = f;
        integrator->integrateFast(500, 10000);
        trappedVals[i] = trap->numTrappedParticles();
        trap->particles_ = particles;
    }

    return trappedVals;
}

void broadScan()
{
    // create random particles
    std::array<Particle, 100> particles = Particle::randomParticles<100>(500);

    // create data vectors
    std::vector<double> fVals = {0.1, 0.4, 0.7};
    std::vector<double> wVVals = linspace(0.2, 2.5, 800);

    // we need to create traps and integrators
    TrapVec traps;
    IntVec integrators;
    createTraps(particles, wVVals, traps, integrators);

    std::ofstream outfile;
    outfile.precision(14);

    Timer timer;
    std::cout << " -- broad scan -- " << std::endl;
    for (auto &f : fVals)
    {
        std::cout << "f=" << f << ":" << std::endl;

        auto trappedVals = parallelInt(particles, traps, integrators, f, wVVals);

        outfile.open(string_format("problem9/data_f%g.dat", f));
        outfile << "total particles: 100\n";
        for (size_t i = 0; i < wVVals.size(); i++)
        {
            outfile << wVVals[i] << " " << trappedVals[i] << "\n";
        }
        outfile.close();

        std::cout << "\n  took " << timer.get_pretty() << std::endl;
        timer.restart();
    }
}

template <uint N>
std::pair<double, double> calcAvgStd(std::array<double, N> vals)
{
    double avg = 0;
    for (size_t i = 0; i < N; i++)
    {
        avg += vals[i];
    }
    avg /= static_cast<double>(N);

    double std = 0;
    for (size_t i = 0; i < N; i++)
    {
        std += (vals[i] - avg)*(vals[i] - avg);
    }
    std = sqrt(std/static_cast<double>(N));

    return {avg, std};
}

void narowScan()
{
    // create random particles
    std::array<Particle, 100> particles = Particle::randomParticles<100>(500);

    // create data vectors
    const size_t wVValCount = 400;
    std::vector<double> wVVals = linspace(0.59, 0.79, wVValCount);
    double f = 0.4;

    const size_t repetitions = 8;

    std::vector<std::array<double, repetitions>> trappedNoInters;
    trappedNoInters.resize(wVValCount);
    std::vector<std::array<double, repetitions>> trappedInters;
    trappedInters.resize(wVValCount);

    // we need to create traps and integrators
    TrapVec traps;
    IntVec integrators;
    createTraps(particles, wVVals, traps, integrators);

    Timer timer;
    std::ofstream outfile;
    outfile.precision(14);

    std::cout << " -- narrow scan -- " << std::endl;
    for (size_t i = 0; i < repetitions; i++)
    {
        std::cout << i + 1 << "/" << repetitions << ":" << std::endl;

        std::cout << "  no interactions:" << std::endl;
        auto trappedVals = parallelInt(particles, traps, integrators, f, wVVals);
        for (size_t j = 0; j < wVValCount; j++)
        {
            trappedNoInters[j][i] = trappedVals[j];
        }

        std::cout << ", took " << timer.get_pretty() << std::endl;
        timer.restart();

        for (size_t i = 0; i < traps.size(); i++)
        {
            traps[i]->enableInteractions();
        }

        std::cout << "  interactions:" << std::endl;
        trappedVals = parallelInt(particles, traps, integrators, f, wVVals);
        for (size_t j = 0; j < wVValCount; j++)
        {
            trappedInters[j][i] = trappedVals[j];
        }

        std::cout << ", " << timer.get_pretty() << std::endl;
        timer.restart();

        if (i != repetitions - 1)
        {
            // generate new particles
            particles = Particle::randomParticles<100>(500);
            for (size_t i = 0; i < wVValCount; i++)
            {
                traps[i]->particles_ = particles;
                traps[i]->disableInteractions();
            }
        }
    }

    outfile.open("problem9/data_narrow_no_interactions.dat");
    outfile << "total particles: 100\n";
    for (size_t i = 0; i < wVValCount; i++)
    {
        auto [avg, std] = calcAvgStd<repetitions>(trappedNoInters[i]);
        outfile << wVVals[i] << " " << avg << " " << std << "\n";
    }
    outfile.close();

    outfile.open("problem9/data_narrow_interactions.dat");
    outfile << "total particles: 100\n";
    for (size_t i = 0; i < wVValCount; i++)
    {
        auto [avg, std] = calcAvgStd<repetitions>(trappedInters[i]);
        outfile << wVVals[i] << " " << avg << " " << std << "\n";
    }
    outfile.close();
}

int main()
{
    broadScan();
    narowScan();

    return 0;
}
