#include "../../../cpp_utils/utils.hpp"

#include <fstream>

int main()
{
    std::ofstream outfile;
    outfile.precision(14);
    outfile.open("src/test/output/data.dat");
    outfile << "hei! :)\n";
    outfile.close();

    return 0;
}
