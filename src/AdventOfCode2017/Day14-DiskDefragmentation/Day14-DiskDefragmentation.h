#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned numSquaresUsed(const std::string& keyString);
unsigned numRegionsOfAdjcacentSquares(const std::string& keyString);

}
