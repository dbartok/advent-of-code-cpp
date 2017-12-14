#pragma once

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned totalScoreForAllGroups(const std::string& groupedString);
unsigned totalGarbageCount(const std::string& groupedString);

}
