#pragma once

#include "IntcodeTextAdventure.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

std::string passwordForMainAirlock(const std::vector<IntcodeNumberType>& intcodeProgram);

}
