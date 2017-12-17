#include "Day05-MazeOfTrampolines.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <vector>
END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<int> inputInstructions;

    int inputOffset;
    while (fileIn >> inputOffset)
    {
        inputInstructions.push_back(inputOffset);
    }

    std::cout << "First part: " << AoC::stepsInstructionsIncreasing(inputInstructions) << std::endl;
    std::cout << "Second part: " << AoC::stepsInstructionsIncreasingDecreasing(inputInstructions) << std::endl;
}
