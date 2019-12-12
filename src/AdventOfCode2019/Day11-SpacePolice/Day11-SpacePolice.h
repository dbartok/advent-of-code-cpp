#pragma once

#include "HullPaintingRobot.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned numPanelsPaintedAtLeastOnce(const std::vector<IntcodeNumberType>& intcodeProgram);
std::string registrationIdentifier(const std::vector<IntcodeNumberType>& intcodeProgram);

}
