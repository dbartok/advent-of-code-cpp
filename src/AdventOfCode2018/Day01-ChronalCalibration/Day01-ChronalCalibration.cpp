#include "Day01-ChronalCalibration.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <numeric>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int sumOfAllNumbers(const std::vector<int>& numbers)
{
    return std::accumulate(numbers.cbegin(), numbers.cend(), 0);
}

}
