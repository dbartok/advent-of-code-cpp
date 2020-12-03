#include "Day03-TobogganTrajectory.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int VERTICAL_STEP = 1;
int HORIZONTAL_STEP = 3;
char TREE = '#';

int numTreesEncountered(const std::vector<std::string>& gridLines)
{
    int xCoord = 0;
    int yCoord = 0;
    int numTrees = 0;

    while (yCoord < gridLines.size())
    {
        if (gridLines.at(yCoord).at(xCoord) == TREE)
        {
            ++numTrees;
        }

        yCoord += VERTICAL_STEP;
        xCoord += HORIZONTAL_STEP;
        xCoord %= gridLines.at(0).size();
    }

    return numTrees;
}

}
