#pragma once

#include <vector>

namespace AdventOfCode
{

using MemoryBanks = std::vector<unsigned>;

unsigned numUniqueRedistributions(MemoryBanks banks);
unsigned cyclesUntilReoccurs(MemoryBanks banks);

}
