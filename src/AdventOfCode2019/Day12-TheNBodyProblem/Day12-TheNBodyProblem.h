#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
unsigned DEFAULT_NUM_STEPS = 1000;
}

namespace AdventOfCode
{

int totalEnergyAfterSteps(const std::vector<std::string>& moonPositionLines, unsigned numSteps = DEFAULT_NUM_STEPS);

}
