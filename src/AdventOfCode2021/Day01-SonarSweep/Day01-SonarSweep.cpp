#include "Day01-SonarSweep.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2021
{
namespace Day01
{

unsigned numDepthMeasurementIncreases(const std::vector<unsigned>& depthMeasurements)
{
    unsigned numIncreases = 0;

    for (auto depthMeasurementIter = std::next(depthMeasurements.cbegin()); depthMeasurementIter < depthMeasurements.cend(); ++depthMeasurementIter)
    {
        if (*depthMeasurementIter > *std::prev(depthMeasurementIter))
        {
            ++numIncreases;
        }
    }

    return numIncreases;
}

}
}
}