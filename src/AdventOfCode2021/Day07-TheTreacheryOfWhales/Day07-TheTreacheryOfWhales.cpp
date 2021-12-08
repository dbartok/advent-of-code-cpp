#include "Day07-TheTreacheryOfWhales.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <algorithm>
#include <numeric>
#include <cmath>
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

std::vector<int> getDistancesFromValue(std::vector<int> vec, int valueToMeasureFrom)
{
    std::transform(vec.begin(), vec.end(), vec.begin(), [valueToMeasureFrom](const auto& elem)
                   {
                       return std::abs(elem - valueToMeasureFrom);
                   });
    return vec;
}

int leastAmountOfFuelSpentToAlign(const std::vector<int>& horizontalPositions)
{
    const int middleValue = getMiddleValue(horizontalPositions);
    const std::vector<int> distancesFromMiddleValue = getDistancesFromValue(horizontalPositions, middleValue);
    return std::accumulate(distancesFromMiddleValue.cbegin(), distancesFromMiddleValue.cend(), 0);
}

int leastAmountOfFuelSpentToAlignWithIncreasingCosts(const std::vector<int>& horizontalPositions)
{
    const auto bounds = std::minmax_element(horizontalPositions.cbegin(), horizontalPositions.cend());

    std::vector<int> possibleFuelCosts;
    for (int alignmentPosition = *bounds.first; alignmentPosition <= *bounds.second; ++alignmentPosition)
    {
        const std::vector<int> distancesFromAlignmentPosition = getDistancesFromValue(horizontalPositions, alignmentPosition);
        const int fuelCost = std::accumulate(distancesFromAlignmentPosition.cbegin(), distancesFromAlignmentPosition.cend(), 0, [](const auto& acc, const auto& elem)
                                             {
                                                 return acc + elem * (elem + 1) / 2;
                                             });
        possibleFuelCosts.push_back(fuelCost);
    }

    return *std::min_element(possibleFuelCosts.cbegin(), possibleFuelCosts.cend());
}

}
}
}