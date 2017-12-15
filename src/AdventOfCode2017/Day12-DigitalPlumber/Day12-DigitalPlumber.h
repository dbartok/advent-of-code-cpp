#pragma once

#include "VillageGraph.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned numNodesInGroupNodeZero(const std::vector<std::string>& neighborsLines);
unsigned numTotalGroups(const std::vector<std::string>& neighborsLines);

}
