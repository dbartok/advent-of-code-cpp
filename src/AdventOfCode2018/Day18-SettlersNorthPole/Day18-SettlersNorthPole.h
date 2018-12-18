#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned totalResourceValueAfterShortTime(const std::vector<std::string>& lumberAreaLines);
unsigned totalResourceValueAfterLongTime(const std::vector<std::string>& lumberAreaLines);

}
