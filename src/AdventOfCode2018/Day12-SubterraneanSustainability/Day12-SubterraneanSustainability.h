#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

long long sumOfPotNumbersWithPlantAfterGrowthShortTime(const std::string& initialStateLine, const std::vector<std::string>& neighborPatternLines);
long long sumOfPotNumbersWithPlantAfterGrowthLongTime(const std::string& initialStateLine, const std::vector<std::string>& neighborPatternLines);

}
