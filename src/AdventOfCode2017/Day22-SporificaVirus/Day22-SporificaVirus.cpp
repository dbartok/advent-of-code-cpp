#include "Day22-SporificaVirus.h"

#include "GridInfectingVirus.h"
#include <AdventOfCode2017/Day03-SpiralMemory/InfiniteGrid.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

InfiniteGrid<bool> createInfiniteGridFromVirusPosLines(const std::vector<std::string>& virusPosLine)
{
    InfiniteGrid<bool> grid;

    const int gridSize = virusPosLine.size();
    const int middleOffset = virusPosLine.size() / 2;

    for (int j = 0; j < gridSize; ++j)
    {
        for (int i = 0; i < gridSize; ++i)
        {
            if (virusPosLine[j][i] == '#')
            {
                grid.setValue(i - middleOffset, j - middleOffset, true);
            }
        }
    }

    return grid;
}

unsigned numBurstsThatAreInfecting(const std::vector<std::string>& initialVirusPosLines, unsigned numIterations)
{
    InfiniteGrid<bool> initialGrid = createInfiniteGridFromVirusPosLines(initialVirusPosLines);

    GridInfectingVirus gridInfectingVirus{initialGrid};
    gridInfectingVirus.infectRepeatedly(numIterations);
    return gridInfectingVirus.getTotalInfectionBursts();
}

}
