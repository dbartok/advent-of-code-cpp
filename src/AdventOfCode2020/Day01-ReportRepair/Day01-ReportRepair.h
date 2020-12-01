#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int productOfTwoEntriesWithGivenSum(const std::vector<int>& entries);
int productOfThreeEntriesWithGivenSum(const std::vector<int>& entries);

}
