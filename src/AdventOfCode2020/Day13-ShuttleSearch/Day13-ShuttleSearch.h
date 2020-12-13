#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int earliestBusIDMultipliedByWaitTime(int earliestDepartureTimestamp, const std::string& scheduleDescriptionString);

}
