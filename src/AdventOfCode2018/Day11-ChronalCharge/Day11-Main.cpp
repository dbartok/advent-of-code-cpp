#include "Day11-ChronalCharge.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    int gridSerialNumber;

    fileIn >> gridSerialNumber;

    std::cout << "First part: " << AoC::highestPowerSmallSquareCoordinates(gridSerialNumber) << std::endl;
}
