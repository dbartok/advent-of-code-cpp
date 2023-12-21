#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

unsigned NUM_STEPS_PART_ONE = 64;

}

namespace AdventOfCode
{
namespace Year2023
{
namespace Day21
{

int numGardenPlotsReachableAfterSteps(const std::vector<std::string>& mapLines, unsigned numSteps = NUM_STEPS_PART_ONE);

}
}
}
