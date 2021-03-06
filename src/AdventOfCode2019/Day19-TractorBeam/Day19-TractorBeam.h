#pragma once

#include <AdventOfCode2019/Day05-SunnyWithAsteroids/IntcodeInterpreter.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int numPointsAffectedByBeam(const std::vector<IntcodeNumberType>& intcodeProgram);
int squareClosestPointChecksum(const std::vector<IntcodeNumberType>& intcodeProgram);

}
