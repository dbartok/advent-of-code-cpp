#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/multiprecision/cpp_int.hpp>

#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using BigNumber = boost::multiprecision::int256_t;

std::vector<unsigned> cardsAfterShuffle(const std::vector<std::string>& shuffleInstructionLines, unsigned deckSize);
BigNumber cardAtPositionAfterMultipleShuffles(const std::vector<std::string>& shuffleInstructionLines, BigNumber deckSize, BigNumber numIterations, BigNumber position);

}
