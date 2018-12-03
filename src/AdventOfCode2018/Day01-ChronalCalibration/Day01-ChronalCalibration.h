#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int sumOfAllNumbers(const std::vector<int>& numbers);
int firstDuplicateSumReached(const std::vector<int>& numbers);

}
