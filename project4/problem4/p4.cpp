#include "ising_computer.hpp"
#include "../../cpp_utils/utils.hpp"

#include <iostream>
#include <fstream>

int main()
{
    std::mt19937 generator;
    generator.seed(8008135);
 
    std::ofstream outfile;
    outfile.precision(14);
    outfile.open("problem4/data.dat");
    Timer timer;
    sampleToFile<2>(1, 100000, outfile, generator);
    std::cout << timer.get_pretty() << std::endl;
    outfile.close();

    return 0;
}
