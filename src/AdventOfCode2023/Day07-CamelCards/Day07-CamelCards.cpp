#include "Day07-CamelCards.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const std::string CARD_STRENGTH_ORDER_ASCENDING = "23456789TJQKA";

}

namespace AdventOfCode
{
namespace Year2023
{
namespace Day07
{

struct PlayerState
{
    std::string hand;
    int bid;
};

PlayerState parseCamelCardLine(const std::string& camelCardsLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, camelCardsLine, boost::is_any_of(" "));

    return {std::move(tokens.at(0)), std::stoi(tokens.at(1))};
}

std::vector<PlayerState> parseCamelCardLines(const std::vector<std::string>& camelCardsLines)
{
    std::vector<PlayerState> playerStates;

    for (const auto& camelCardsLine : camelCardsLines)
    {
        PlayerState playerState = parseCamelCardLine(camelCardsLine);
        playerStates.push_back(std::move(playerState));
    }

    return playerStates;
}

std::vector<int> getFrequenciesForHand(const std::string& hand)
{
    std::unordered_map<char, int> charToFrequency;
    for (const char c : hand)
    {
        ++charToFrequency[c];
    }

    std::vector<int> frequencies;
    for (const auto& charAndFrequency : charToFrequency)
    {
        frequencies.push_back(charAndFrequency.second);
    }
    std::sort(frequencies.begin(), frequencies.end(), std::greater<int>());
    return frequencies;
}

bool isFirstHandWeakerTypeThanSecondHand(const std::string& firstHand, const std::string& secondHand)
{
    const std::vector<int> firstHandFrequencies = getFrequenciesForHand(firstHand);
    const std::vector<int> secondHandFrequencies = getFrequenciesForHand(secondHand);

    for (int i = 0; i < firstHandFrequencies.size(); ++i)
    {
        if (firstHandFrequencies.at(i) < secondHandFrequencies.at(i))
        {
            return true;
        }
        else if (firstHandFrequencies.at(i) > secondHandFrequencies.at(i))
        {
            return false;
        }
    }

    return false;
}

bool isFirstHandWeakerCardOrderThanSecondHand(const std::string& firstHand, const std::string& secondHand)
{
    for (int i = 0; i < firstHand.size(); ++i)
    {
        const int firstHandCurrentCharValue = CARD_STRENGTH_ORDER_ASCENDING.find(firstHand.at(i));
        const int secondHandCurrentCharValue = CARD_STRENGTH_ORDER_ASCENDING.find(secondHand.at(i));

        if (firstHandCurrentCharValue < secondHandCurrentCharValue)
        {
            return true;
        }
        else if (firstHandCurrentCharValue > secondHandCurrentCharValue)
        {
            return false;
        }
    }

    throw std::runtime_error("Hand strength ordering cannot be determined");
}

bool isFirstHandWeakerThanSecondHand(const std::string& firstHand, const std::string& secondHand)
{
    if (isFirstHandWeakerTypeThanSecondHand(firstHand, secondHand))
    {
        return true;
    }

    if (isFirstHandWeakerTypeThanSecondHand(secondHand, firstHand))
    {
        return false;
    }

    return isFirstHandWeakerCardOrderThanSecondHand(firstHand, secondHand);
}

int totalWinnings(const std::vector<std::string>& camelCardsLines)
{
    std::vector<PlayerState> playerStates = parseCamelCardLines(camelCardsLines);

    std::sort(playerStates.begin(), playerStates.end(), [](const auto& lhs, const auto& rhs)
              {
                  return isFirstHandWeakerThanSecondHand(lhs.hand, rhs.hand);
              });

    int totalWinnings = 0;
    for (int i = 0; i < playerStates.size(); ++i)
    {
        totalWinnings += (i + 1) * playerStates.at(i).bid;
    }
    return totalWinnings;
}

}
}
}