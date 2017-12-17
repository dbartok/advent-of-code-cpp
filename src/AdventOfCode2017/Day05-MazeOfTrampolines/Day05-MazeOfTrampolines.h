#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned stepsInstructionsIncreasing(const std::vector<int>& instructions);
unsigned stepsInstructionsIncreasingDecreasing(const std::vector<int>& instructions);

}
