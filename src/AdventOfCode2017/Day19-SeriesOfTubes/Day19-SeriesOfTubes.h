#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

std::string lettersVisited(const std::vector<std::string>& diagramLines);
unsigned totalNumStepsForPacket(const std::vector<std::string>& diagramLines);

}
