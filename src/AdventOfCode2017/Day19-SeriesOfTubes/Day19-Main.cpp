#include "Day19-SeriesOfTubes.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<std::string> inputDiagramLines;

    std::string inputDiagramLine;
    while (std::getline(fileIn, inputDiagramLine))
    {
        inputDiagramLines.push_back(std::move(inputDiagramLine));
    }

    std::cout << "First part: " << AoC::lettersVisited(inputDiagramLines) << std::endl;
    std::cout << "Second part: " << AoC::totalNumStepsForPacket(inputDiagramLines) << std::endl;
}
