#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int numStepsFromStartAtEnd(const std::vector<std::string>& path);
int maxNumStepsFromStart(const std::vector<std::string>& path);

}
