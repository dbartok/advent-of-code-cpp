#include "Day23-CrabCups.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::string initialCupLabellingString;

    fileIn >> initialCupLabellingString;

    std::cout << "First part: " << AoC::cupLabelsStartingFromCupOne(initialCupLabellingString, 100) << std::endl;
}
