#include "Day01-TyrannyOfTheRocket.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

constexpr int getFuelRequirement(int moduleMass) noexcept
{
    return (moduleMass / 3) - 2;
}

int getSumOfFuelRequirements(const std::vector<int>& moduleMasses)
{
    return std::accumulate(moduleMasses.cbegin(), moduleMasses.cend(), 0,
                           [](int acc, int mass) noexcept
                           {
                               return acc + getFuelRequirement(mass);
                           });
}

}
