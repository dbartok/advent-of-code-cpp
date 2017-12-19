#include "Day18-Duet.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<std::string> inputInstructionStrings;

    std::string lineBuffer;
    while (std::getline(fileIn, lineBuffer))
    {
        inputInstructionStrings.push_back(std::move(lineBuffer));
    }

    std::cout << "First part: " << AoC::valueOfFirstRecoveredFrequency(inputInstructionStrings) << std::endl;
}
