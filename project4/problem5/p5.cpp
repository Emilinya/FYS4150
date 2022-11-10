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

    size_t cycles = 5000;

    outfile.open("problem5/data-T1-unordered.dat");
    Timer timer;
    sampleToFile<20>(1, cycles, outfile, generator, InitialState::UNORDERED);
    std::cout << timer.get_pretty() << std::endl;
    outfile.close();

    outfile.open("problem5/data-T1-ordered.dat");
    timer.restart();
    sampleToFile<20>(1, cycles, outfile, generator, InitialState::ORDERED);
    std::cout << timer.get_pretty() << std::endl;
    outfile.close();

    outfile.open("problem5/data-T2.4-unordered.dat");
    timer.restart();
    sampleToFile<20>(2.4, cycles, outfile, generator, InitialState::UNORDERED);
    std::cout << timer.get_pretty() << std::endl;
    outfile.close();

    outfile.open("problem5/data-T2.4-ordered.dat");
    timer.restart();
    sampleToFile<20>(2.4, cycles, outfile, generator, InitialState::ORDERED);
    std::cout << timer.get_pretty() << std::endl;
    outfile.close();

    return 0;
}
