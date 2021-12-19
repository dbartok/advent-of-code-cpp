#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2021
{
namespace Day18
{

int magnitudeOfFinalSum(const std::vector<std::string>& snailfishNumberStringLines);
int largestMagnitudeOfAnyTwoAddendSum(const std::vector<std::string>& snailfishNumberStringLines);

}
}
}
