#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int biodiversityRatingOfFirstDuplicateLayout(const std::vector<std::string>& initialLayoutLines);
unsigned numBugsAfterSimulationRecursiveGrid(const std::vector<std::string>& initialLayoutLines, unsigned numSteps);

}
