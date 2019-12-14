#pragma once

#include "ArcadeCabinet.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned numBlockTilesWhenGameExits(const std::vector<IntcodeNumberType>& intcodeProgram);
unsigned scoreAfterLastBlockBroken(const std::vector<IntcodeNumberType>& intcodeProgram);

}
