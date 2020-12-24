#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int numTilesWithBlackSideUpInInitialState(const std::vector<std::string>& tilePathLines);
int numTilesWithBlackSideUpAfterMultipleDays(const std::vector<std::string>& tilePathLines);

}
