#include "../problem4/ising_computer.hpp"
#include "../../cpp_utils/utils.hpp"

#include <iostream>
#include <fstream>

int main()
{
    std::mt19937 generator;
    generator.seed(8008135);

    std::ofstream outfile;
    outfile.precision(14);

    size_t cycles = 10000;

    outfile.open("problem6/data-T1.dat");
    Timer timer;
    sampleToFile(20, 1, cycles, outfile, generator, InitialState::UNORDERED, 300);
    std::cout << timer.get_pretty() << std::endl;
    outfile.close();

    outfile.open("problem6/data-T2.4.dat");
    timer.restart();
    sampleToFile(20, 2.4, cycles, outfile, generator, InitialState::UNORDERED, 300);
    std::cout << timer.get_pretty() << std::endl;
    outfile.close();

    return 0;
}
