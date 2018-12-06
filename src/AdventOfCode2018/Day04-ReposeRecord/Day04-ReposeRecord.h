#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned guardMostMinutesSleptTimesMinute(const std::vector<std::string>& eventLines);
unsigned guardMostFrequentlyMinuteSleptTimesMinute(const std::vector<std::string>& eventLines);

}
