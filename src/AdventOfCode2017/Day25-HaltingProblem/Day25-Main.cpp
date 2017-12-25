#include "Day25-HaltingProblem.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<std::string> inputTuringMachineLines;

    std::string lineBuffer;
    while (std::getline(fileIn, lineBuffer))
    {
        inputTuringMachineLines.push_back(std::move(lineBuffer));
    }

    std::cout << "First part: " << AoC::diagnosticChecksum(inputTuringMachineLines) << std::endl;
}
