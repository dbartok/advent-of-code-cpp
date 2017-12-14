#include "Day10-KnotHash.h"

#include "KnotHasher.h"
#include "../../Common/Utils.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");

    std::string inputLengthsString;
    std::getline(fileIn, inputLengthsString);

    std::vector<unsigned> inputLengths;
    AoC::Utils::splitStringIntoTypedVector(inputLengthsString, inputLengths, ',');

    std::cout << "First part: " << AoC::firstTwoMultipliedAfterKnotting(AoC::KnotHasher::STANDARD_LIST_SIZE, inputLengths) << std::endl;
    std::cout << "Second part: " << AoC::knotHashDenseString(inputLengthsString) << std::endl;
}
