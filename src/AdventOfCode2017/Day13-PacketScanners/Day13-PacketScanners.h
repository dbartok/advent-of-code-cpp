#pragma once

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned severityOfWholeTrip(const std::vector<std::string>& scannerRangeLines);
unsigned smallestDelayNotToGetCaught(const std::vector<std::string>& scannerRangeLines);

}
