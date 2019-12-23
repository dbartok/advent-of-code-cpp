#pragma once

#include "SlamShuffler.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

std::vector<unsigned> cardsAfterShuffle(const std::vector<std::string>& shuffleInstructionLines, unsigned deckSize);
BigNumber cardAtPositionAfterMultipleShuffles(const std::vector<std::string>& shuffleInstructionLines, BigNumber deckSize, BigNumber numIterations, BigNumber position);

}
