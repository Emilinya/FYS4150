#include "../problem4/ising_computer.hpp"
#include "../../cpp_utils/utils.hpp"

#include <iostream>
#include <fstream>
#include <omp.h>

template <uint32_t L>
void doCalc(size_t nTs)
{
    auto Tray = linspace(2.1, 2.4, nTs);

    std::vector<double> epsRay;
    epsRay.resize(nTs);
    std::vector<double> absmRay;
    absmRay.resize(nTs);
    std::vector<double> CvRay;
    CvRay.resize(nTs);
    std::vector<double> Xray;
    Xray.resize(nTs);

    Timer timer;
    size_t repeats = 16;
    size_t cycles = 320000;
    std::cout << " --- L=" << L << " --- " << std::endl;

    std::ofstream outfile;
    outfile.precision(14);
    outfile.open(string_format("problem8/data-L%d.dat", L));
    outfile << "repeats=" << repeats << ", nTs=" << nTs << "\n";

    for (size_t i = 0; i < repeats; i++)
    {
        outfile << "--------------------" << "\n";

        #pragma omp parallel for
        for (size_t j = 0; j < nTs; j++)
        {
            std::mt19937 generator;
            generator.seed(8008135 + i * 42 + j * 1337 * L);

            if (omp_get_thread_num() == 0)
            {
                int numThreads = omp_get_num_threads();
                std::cout << "\r  " << i * nTs / numThreads + j + 1 << "/" << repeats * nTs / numThreads;
                fflush(stdout);
            }

            auto [avgEps, avgAbsm, Cv, X] = calcQtys<L>(Tray[j], cycles, generator, InitialState::UNORDERED, 2000);
            epsRay[j] = avgEps;
            absmRay[j] = avgAbsm;
            CvRay[j] = Cv;
            Xray[j] = X;
        }

        for (size_t j = 0; j < nTs; j++)
        {
            outfile << Tray[j] << " " << epsRay[j] << " " << absmRay[j] << " " << CvRay[j] << " " << Xray[j] << "\n";
        }
    }

    std::cout << "\n" << timer.get_pretty() << "\n" << std::endl;

    outfile.close();
}

int main()
{
    const size_t nTs = 160;

    // is there a better way of doing this?
    doCalc<40>(nTs);
    doCalc<60>(nTs);
    doCalc<80>(nTs);
    doCalc<100>(nTs);

    return 0;
}
