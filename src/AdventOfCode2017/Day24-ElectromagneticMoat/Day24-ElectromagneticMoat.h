#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned strengthOfStrongestBridge(const std::vector<std::string>& componentLines);
unsigned strengthOfStrongestLongestBridge(const std::vector<std::string>& componentLines);

}
