#include "Day07-TheTreacheryOfWhales.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <algorithm>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2021
{
namespace Day07
{

int getMiddleValue(std::vector<int> vec)
{
        const size_t middleElementIndex = vec.size() / 2;
        std::nth_element(vec.begin(), vec.begin() + middleElementIndex, vec.end());
        return vec.at(middleElementIndex);
}

int leastAmountOfFuelSpentToAlign(const std::vector<int>& horizontalPositions)
{
    const int middleValue = getMiddleValue(horizontalPositions);
    const int totalFuelSpent = std::accumulate(horizontalPositions.cbegin(), horizontalPositions.cend(), 0, [middleValue](const auto& acc, const auto& elem)
                                               {
                                                   return acc + std::abs(elem - middleValue);
                                               });
    return totalFuelSpent;
}

}
}
}