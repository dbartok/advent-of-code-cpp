#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned judgeFinalCountNoCriteria(unsigned aStart, unsigned bStart);
unsigned judgeFinalCountWithCriteria(unsigned aStart, unsigned bStart);

unsigned generatorStartFromLine(const std::string& line);

}
