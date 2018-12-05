#include "Day05-AlchemicalReaction.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::string line;

    fileIn >> line;

    std::cout << "First part: " << AoC::getReducedPolymerLength(line) << std::endl;
    std::cout << "Second part: " << AoC::getOneUnitRemovedShortestReducedLength(line) << std::endl;
}
