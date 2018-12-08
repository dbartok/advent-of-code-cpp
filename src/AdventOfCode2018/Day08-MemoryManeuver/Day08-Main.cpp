#include "Day08-MemoryManeuver.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <vector>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");

    std::string line;
    std::getline(fileIn, line);

    std::vector<unsigned> nodeNumbers;
    AoC::Utils::splitStringIntoTypedVector(line, nodeNumbers, ' ');

    std::cout << "First part: " << AoC::sumOfAllMetadataEntries(nodeNumbers) << std::endl;
}
