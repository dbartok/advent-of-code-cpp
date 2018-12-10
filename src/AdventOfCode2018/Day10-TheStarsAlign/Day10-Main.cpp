#include "Day10-TheStarsAlign.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");

    std::string lineBuffer;
    std::vector<std::string> lines;
    while (std::getline(fileIn, lineBuffer))
    {
        lines.push_back(std::move(lineBuffer));
    }

    std::cout << "First part: " << std::endl << AoC::serializedMessageWhenAligned(lines) << std::endl << std::endl;
    std::cout << "Second part: " << AoC::timeTakenUntilAligned(lines) << std::endl;
}
