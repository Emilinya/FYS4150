#include "../problem4/ising_computer.hpp"
#include "../../cpp_utils/utils.hpp"

#include <iostream>
#include <fstream>
#include <omp.h>

void doCalc(uint32_t L, size_t nTs)
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
    size_t repeats = 8;
    size_t cycles = 160000;
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

            auto [avgEps, avgAbsm, Cv, X] = calcQtys(L, Tray[j], cycles, generator, InitialState::UNORDERED, 2000);
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
    std::array Lvals{40, 60, 80, 100};
    for (auto &L : Lvals)
    {
        doCalc(L, 80);
    }

    return 0;
}
