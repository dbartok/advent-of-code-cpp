#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int numStepsToExit(const std::vector<std::string>& donutMazeLines);
int numStepsToExitWithMatchingLayers(const std::vector<std::string>& donutMazeLines);

}