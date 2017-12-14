#pragma once

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int numStepsFromStartAtEnd(const std::vector<std::string>& path);
int maxNumStepsFromStart(const std::vector<std::string>& path);

}
