#include "Day24-ElectromagneticMoat.h"

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
    std::vector<std::string> inputComponentLines;

    std::string lineBuffer;

    while (std::getline(fileIn, lineBuffer))
    {
        inputComponentLines.push_back(std::move(lineBuffer));
    }

    std::cout << "First part: " << AoC::strengthOfStrongestBridge(inputComponentLines) << std::endl;
    std::cout << "Second part: " << AoC::strengthOfStrongestLongestBridge(inputComponentLines) << std::endl;
}
