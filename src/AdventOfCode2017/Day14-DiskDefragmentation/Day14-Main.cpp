#include "Day14-DiskDefragmentation.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::string inputKeyString;

    fileIn >> inputKeyString;

    std::cout << "First part: " << AoC::numSquaresUsed(inputKeyString) << std::endl;
    std::cout << "Second part: " << AoC::numRegionsOfAdjcacentSquares(inputKeyString) << std::endl;
}
