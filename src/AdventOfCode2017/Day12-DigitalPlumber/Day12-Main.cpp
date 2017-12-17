#include "Day12-DigitalPlumber.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

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
    std::vector<std::string> inputNeighborsLines;

    std::string lineBuffer;
    while (std::getline(fileIn, lineBuffer))
    {
        inputNeighborsLines.push_back(lineBuffer);
    }

    std::cout << "First part: " << AoC::numNodesInGroupNodeZero(inputNeighborsLines) << std::endl;
    std::cout << "Second part: " << AoC::numTotalGroups(inputNeighborsLines) << std::endl;
}
