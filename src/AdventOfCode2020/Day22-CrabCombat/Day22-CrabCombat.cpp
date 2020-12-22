#include "Day22-CrabCombat.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <deque>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class CombatCardGameSimulator
{
public:
    CombatCardGameSimulator(std::deque<int> startingDeckOne, std::deque<int> startingDeckTwo)
        : m_deckOne{std::move(startingDeckOne)}
        , m_deckTwo{std::move(startingDeckTwo)}
    {

    }

    void playGame()
    {
        while (!isGameOver())
        {
            auto& winnerThisRound = m_deckOne;
            auto& loserThisRound = m_deckTwo;
            if (winnerThisRound.front() < loserThisRound.front())
            {
                std::swap(winnerThisRound, loserThisRound);
            }

            winnerThisRound.push_back(winnerThisRound.front());
            winnerThisRound.pop_front();

            winnerThisRound.push_back(loserThisRound.front());
            loserThisRound.pop_front();
        }
    }

    int getWinningPlayerScore() const
    {
        assert(isGameOver());

        const auto& winningDeck = m_deckOne.empty() ? m_deckTwo : m_deckOne;
        int multiplier = 1;

        return std::accumulate(winningDeck.crbegin(), winningDeck.crend(), 0, [&multiplier](int acc, int card)
                               {
                                   return acc + (multiplier++) * card;
                               });

    }

private:
    std::deque<int> m_deckOne;
    std::deque<int> m_deckTwo;

    bool isGameOver() const
    {
        return m_deckOne.empty() || m_deckTwo.empty();
    }
};

std::deque<int> createDeck(const std::vector<std::string>& startingDeckSection)
{
    std::deque<int> deck;

    for (auto lineIter = std::next(startingDeckSection.cbegin()); lineIter != startingDeckSection.cend(); ++lineIter)
    {
        deck.push_back(std::stoi(*lineIter));
    }

    return deck;
}

CombatCardGameSimulator creatCombatCardGameSimulator(const std::vector<std::string>& startingDecksLines)
{
    std::vector<std::vector<std::string>> startingDeckSections;
    boost::split(startingDeckSections, startingDecksLines, [](const auto& line) {return line.empty(); });

    std::deque<int> deckOne = createDeck(startingDeckSections.at(0));
    std::deque<int> deckTwo = createDeck(startingDeckSections.at(1));

    return CombatCardGameSimulator{std::move(deckOne), std::move(deckTwo)};

}

int winningPlayerScore(const std::vector<std::string>& startingDecksLines)
{
    CombatCardGameSimulator simulator = creatCombatCardGameSimulator(startingDecksLines);
    simulator.playGame();
    return simulator.getWinningPlayerScore();
}

}
