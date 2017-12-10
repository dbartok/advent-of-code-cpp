#include "Day10-KnotHash.h"

#include "Knotter.h"
#include "../../Common/Utils.h"

#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

namespace
{
    size_t STANDARD_LIST_SIZE = 256;
}

namespace AdventOfCode
{

unsigned firstTwoMultipliedAfterKnotting(size_t listSize, const std::vector<unsigned>& lengths)
{
    Knotter knotter{listSize, lengths};
    knotter.execute();
    return knotter.firstTwoMultiplied();
}

}


int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");

    std::string inputLengthsString;
    std::getline(fileIn, inputLengthsString);

    std::vector<unsigned> inputLengths;
    AoC::Utils::splitStringIntoTypedVector(inputLengthsString, inputLengths, ',');

    std::cout << "First part: " << AoC::firstTwoMultipliedAfterKnotting(STANDARD_LIST_SIZE, inputLengths) << std::endl;
}
