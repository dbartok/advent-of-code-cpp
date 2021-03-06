#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned riskLevelOfSmallestArea(const std::vector<std::string>& depthAndTargetLines);
unsigned fewestMinutesToTarget(const std::vector<std::string>& depthAndTargetLines);

}
