#include "../problem4/ising_computer.hpp"
#include "../../cpp_utils/utils.hpp"

#include <iostream>
#include <fstream>
#include <omp.h>

void doCalc(uint32_t L, size_t nTs) {
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
    size_t cycles = 3000;
    std::cout << " --- L=" << L << " --- " << std::endl;

    #pragma omp parallel for
    for (size_t i = 0; i < nTs; i++)
    {
        if (omp_get_thread_num() == 0)
        {
            std::cout << "\r  " << i + 1 << "/" << nTs / omp_get_num_threads();
            fflush(stdout);
        }

        auto [avgEps, avgAbsm, Cv, X] = calcQtys(L, Tray[i], cycles, InitialState::UNORDERED, 300);
        epsRay[i] = avgEps;
        absmRay[i] = avgAbsm;
        CvRay[i] = Cv;
        Xray[i] = X;
    }

    std::cout << "\n" << timer.get_pretty() << "\n" << std::endl;
    

    std::ofstream outfile;
    outfile.precision(14);
    outfile.open(string_format("problem8/data-L%d.dat", L));
    for (size_t i = 0; i < nTs; i++)
    {
        outfile << Tray[i] << " " << epsRay[i] << " " << absmRay[i] << " " << CvRay[i] << " " << Xray[i] << "\n";
    }
    
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
