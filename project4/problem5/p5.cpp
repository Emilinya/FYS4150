#include "../problem4/ising_computer.hpp"
#include "../../cpp_utils/utils.hpp"

#include <iostream>
#include <fstream>

int main()
{
    std::ofstream outfile;
    outfile.precision(14);

    size_t cycles = 2000;

    outfile.open("problem5/data-T1-unordered.dat");
    Timer timer;
    sampleToFile(20, 1, cycles, outfile, InitialState::UNORDERED);
    std::cout << timer.get_pretty() << std::endl;
    outfile.close();

    outfile.open("problem5/data-T1-ordered.dat");
    timer.restart();
    sampleToFile(20, 1, cycles, outfile, InitialState::ORDERED);
    std::cout << timer.get_pretty() << std::endl;
    outfile.close();

    outfile.open("problem5/data-T2.4-unordered.dat");
    timer.restart();
    sampleToFile(20, 2.4, cycles, outfile, InitialState::UNORDERED);
    std::cout << timer.get_pretty() << std::endl;
    outfile.close();

    outfile.open("problem5/data-T2.4-ordered.dat");
    timer.restart();
    sampleToFile(20, 2.4, cycles, outfile, InitialState::ORDERED);
    std::cout << timer.get_pretty() << std::endl;
    outfile.close();

    return 0;
}
