#include "Day10-AdapterArray.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int numGivenMagnitudeDifferences(const std::vector<int>& vec, int targetDifference)
{
    int result = 0;
    for (auto iter = vec.cbegin(); iter != std::prev(vec.cend()); ++iter)
    {
        if (*std::next(iter) - *iter == targetDifference)
        {
            ++result;
        }
    }

    return result;
}

int numOneAndThreeJoltDifferencesMultiplied(std::vector<int> joltageRatings)
{
    std::sort(joltageRatings.begin(), joltageRatings.end());

    joltageRatings.insert(joltageRatings.begin() , 0);
    joltageRatings.insert(joltageRatings.end(), joltageRatings.back() + 3);

    int numOneMagnitudeDifferences = numGivenMagnitudeDifferences(joltageRatings, 1);
    int numThreeMagnitudeDifferences = numGivenMagnitudeDifferences(joltageRatings, 3);

    return numOneMagnitudeDifferences * numThreeMagnitudeDifferences;
}

}
