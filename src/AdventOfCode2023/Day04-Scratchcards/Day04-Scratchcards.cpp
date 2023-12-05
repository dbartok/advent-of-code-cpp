#include "Day04-Scratchcards.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2023
{
namespace Day04
{

using Numbers = std::vector<int>;
using Scratchcard = std::pair<Numbers, Numbers>;

Numbers parseNumbersSegment(std::string numbersSegment)
{
    boost::trim(numbersSegment);
    Numbers numbers;
    AdventOfCode::Utils::splitStringIntoTypedVector(numbersSegment, numbers);
    return numbers;
}

Scratchcard parseScratchcardLine(const std::string& scratchcardLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, scratchcardLine, boost::is_any_of(":|"));

    Numbers winningNumbers = parseNumbersSegment(tokens.at(1));
    Numbers playedNumbers = parseNumbersSegment(tokens.at(2));

    return {winningNumbers, playedNumbers};
}


std::vector<Scratchcard> parseScratchcardLines(const std::vector<std::string>& scratchcardLines)
{
    std::vector<Scratchcard> scratchcards;

    for (const auto& scratchcardLine : scratchcardLines)
    {
        Scratchcard scratchcard = parseScratchcardLine(scratchcardLine);
        scratchcards.push_back(std::move(scratchcard));
    }

    return scratchcards;
}

int getValue(const Scratchcard& scratchcard)
{
    Numbers winningNumbers = scratchcard.first;
    std::sort(winningNumbers.begin(), winningNumbers.end());

    Numbers playedNumbers = scratchcard.second;
    std::sort(playedNumbers.begin(), playedNumbers.end());

    Numbers successfullyGuessedNumbers;
    std::set_intersection(winningNumbers.cbegin(), winningNumbers.cend(),
                          playedNumbers.cbegin(), playedNumbers.cend(),
                          std::back_inserter(successfullyGuessedNumbers));

    const int pointExponent = std::max(successfullyGuessedNumbers.size() - 1, 0u);
    return static_cast<int>(std::pow(2, pointExponent));
}


int numPointsWorthInTotal(const std::vector<std::string>& scratchcardLines)
{
    const std::vector<Scratchcard> scratchcards = parseScratchcardLines(scratchcardLines);

    return std::accumulate(scratchcards.cbegin(), scratchcards.cend(), 0, [](int acc, const auto& scratchcard)
                           {
                               return acc + getValue(scratchcard);
                           });
}

}
}
}