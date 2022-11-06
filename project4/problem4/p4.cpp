#include "ising_computer.hpp"
#include "../../cpp_utils/utils.hpp"

#include <iostream>
#include <fstream>

int main()
{
    std::ofstream outfile;
    outfile.precision(14);
    outfile.open("problem4/data.dat");
    Timer timer;
    sampleToFile(2, 1, 5000, outfile);
    std::cout << timer.get_pretty() << std::endl;
    outfile.close();

    return 0;
}
