#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using MemoryBanks = std::vector<unsigned>;

unsigned numUniqueRedistributions(MemoryBanks banks);
unsigned cyclesUntilReoccurs(MemoryBanks banks);

}
