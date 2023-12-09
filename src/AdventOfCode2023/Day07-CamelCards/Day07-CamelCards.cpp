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

class HandComparator
{
public:
    HandComparator(char joker = '\0')
        : m_joker{joker}
    {

    }

    bool operator()(const std::string& firstHand, const std::string& secondHand) const
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

private:
    char m_joker;

    bool isFirstHandWeakerTypeThanSecondHand(const std::string& firstHand, const std::string& secondHand) const
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

    bool isFirstHandWeakerCardOrderThanSecondHand(const std::string& firstHand, const std::string& secondHand) const
    {
        for (int i = 0; i < firstHand.size(); ++i)
        {
            const int firstHandCurrentCardValue = getCardValue(firstHand.at(i));
            const int secondHandCurrentCardValue = getCardValue(secondHand.at(i));

            if (firstHandCurrentCardValue < secondHandCurrentCardValue)
            {
                return true;
            }
            else if (firstHandCurrentCardValue > secondHandCurrentCardValue)
            {
                return false;
            }
        }

        throw std::runtime_error("Hand strength ordering cannot be determined");
    }

    std::vector<int> getFrequenciesForHand(const std::string& hand) const
    {
        std::unordered_map<char, int> cardToFrequency;
        for (const char card : hand)
        {
            ++cardToFrequency[card];
        }

        int jokerFrequency = 0;
        const auto jokerFrequencyIter = cardToFrequency.find(m_joker);
        if (jokerFrequencyIter != cardToFrequency.cend())
        {
            jokerFrequency = jokerFrequencyIter->second;
            cardToFrequency.erase(jokerFrequencyIter);
        }

        std::vector<int> frequencies;
        for (const auto& cardAndFrequency : cardToFrequency)
        {
            frequencies.push_back(cardAndFrequency.second);
        }
        std::sort(frequencies.begin(), frequencies.end(), std::greater<int>());

        frequencies.front() += jokerFrequency;

        return frequencies;
    }

    int getCardValue(char card) const
    {
        if (card == m_joker)
        {
            return -1;
        }

        return CARD_STRENGTH_ORDER_ASCENDING.find(card);
    }
};

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

int totalWinnings(const std::vector<std::string>& camelCardsLines, const HandComparator& handComparator)
{
    std::vector<PlayerState> playerStates = parseCamelCardLines(camelCardsLines);

    std::sort(playerStates.begin(), playerStates.end(), [&handComparator](const auto& lhs, const auto& rhs)
              {
                  return handComparator(lhs.hand, rhs.hand);
              });

    int totalWinnings = 0;
    for (int i = 0; i < playerStates.size(); ++i)
    {
        totalWinnings += (i + 1) * playerStates.at(i).bid;
    }
    return totalWinnings;
}

int totalWinnings(const std::vector<std::string>& camelCardsLines)
{
    return totalWinnings(camelCardsLines, HandComparator{});
}

int totalWinningsWithJokers(const std::vector<std::string>& camelCardsLines)
{
    return totalWinnings(camelCardsLines, HandComparator{'J'});
}

}
}
}