#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int sumOfAllNumbers(const std::vector<int>& numbers);
int firstDuplicateSumReached(const std::vector<int>& numbers);

}
