#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int64_t numTreesEncounteredSingleSlope(const std::vector<std::string>& gridLines);
int64_t numTreesEncounteredMultipleSlopes(const std::vector<std::string>& gridLines);

}
