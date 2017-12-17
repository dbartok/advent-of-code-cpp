#include "Day12-DigitalPlumber.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned numNodesInGroupNodeZero(const std::vector<std::string>& neighborsLines)
{
    VillageGraph villageGraph = VillageGraph::fromNeighborsLines(neighborsLines);
    return villageGraph.numNodesInGroupContainingNode(0);
}

unsigned numTotalGroups(const std::vector<std::string>& neighborsLines)
{
    VillageGraph villageGraph = VillageGraph::fromNeighborsLines(neighborsLines);
    return villageGraph.numTotalGroups();
}

}
