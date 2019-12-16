#pragma once

#include "OxygenRepairDroid.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int fewestNumberOfMovementsToOxygen(const std::vector<IntcodeNumberType>& intcodeProgram);
int numMinutesUntilOxygenSpreads(const std::vector<IntcodeNumberType>& intcodeProgram);

}
