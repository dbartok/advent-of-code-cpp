#include "Day22-CrabCombat.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash/hash.hpp>
#include <boost/optional.hpp>

#include <deque>
#include <numeric>
#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Deck = std::deque<int>;
using DeckPair = std::pair<Deck, Deck>;

enum class Player
{
    ONE,
    TWO,
};

enum class Action
{
    NONE,
    TERMINATE_GAME,
};

struct RoundResult
{
    RoundResult(Player winningPlayer, Action action = Action::NONE)
        : winningPlayer{winningPlayer}
        , action(action)
    {

    }

    Player winningPlayer;
    Action action;
};

class CombatCardGameSimulator
{
public:
    CombatCardGameSimulator(DeckPair startingDecks)
        : m_decks{std::move(startingDecks)}
    {

    }

    Player determineWinningPlayer()
    {
        while (true)
        {
            RoundResult roundResult = determineRoundResult();
            if (roundResult.action == Action::TERMINATE_GAME)
            {
                return roundResult.winningPlayer;
            }

            Deck* winnerDeckThisRound = &m_decks.first;
            Deck* loserDeckThisRound = &m_decks.second;
            if (roundResult.winningPlayer != Player::ONE)
            {
                std::swap(winnerDeckThisRound, loserDeckThisRound);
            }

            winnerDeckThisRound->push_back(winnerDeckThisRound->front());
            winnerDeckThisRound->pop_front();

            winnerDeckThisRound->push_back(loserDeckThisRound->front());
            loserDeckThisRound->pop_front();
        }
    }

    int getPlayerScore(Player player) const
    {
        const auto& deck = (player == Player::ONE ? m_decks.first : m_decks.second);
        int multiplier = 1;

        return std::accumulate(deck.crbegin(), deck.crend(), 0, [&multiplier](int acc, int card)
                               {
                                   return acc + (multiplier++) * card;
                               });

    }

protected:
    DeckPair m_decks;

    virtual RoundResult determineRoundResult() const
    {
        if (m_decks.first.empty() || m_decks.second.empty())
        {
            return {m_decks.second.empty() ? Player::ONE : Player::TWO, Action::TERMINATE_GAME};
        }

        return m_decks.first.front() > m_decks.second.front() ? Player::ONE : Player::TWO;
    }
};

class RecursiveCombatCardGameSimulator : public CombatCardGameSimulator
{
public:
    using CombatCardGameSimulator::CombatCardGameSimulator;

protected:
    RoundResult determineRoundResult() const override
    {
        bool wasInserted = m_previouslySeenStates.insert(m_decks).second;
        if (!wasInserted)
        {
            return {Player::ONE, Action::TERMINATE_GAME};
        }

        if (m_decks.first.empty() || m_decks.second.empty())
        {
            return {m_decks.second.empty() ? Player::ONE : Player::TWO, Action::TERMINATE_GAME};
        }

        const bool areThereEnoughCardsToRecurse = m_decks.first.front() < m_decks.first.size() && m_decks.second.front() < m_decks.second.size();
        if (!areThereEnoughCardsToRecurse)
        {
            return CombatCardGameSimulator::determineRoundResult();
        }

        Deck newFirstDeck{m_decks.first.cbegin() + 1, m_decks.first.cbegin() + m_decks.first.front() + 1};
        Deck newSecondDeck{m_decks.second.cbegin() + 1, m_decks.second.cbegin() + m_decks.second.front() + 1};
        DeckPair newDeckPair{std::move(newFirstDeck), std::move(newSecondDeck)};
        RecursiveCombatCardGameSimulator recursiveSimulator{std::move(newDeckPair)};

        return recursiveSimulator.determineWinningPlayer();

    }

private:
    mutable std::unordered_set<DeckPair, boost::hash<DeckPair>> m_previouslySeenStates;
};

Deck createDeck(const std::vector<std::string>& startingDeckSection)
{
    Deck deck;

    for (auto lineIter = std::next(startingDeckSection.cbegin()); lineIter != startingDeckSection.cend(); ++lineIter)
    {
        deck.push_back(std::stoi(*lineIter));
    }

    return deck;
}

DeckPair parseStartingDecksLines(const std::vector<std::string>& startingDecksLines)
{
    std::vector<std::vector<std::string>> startingDeckSections;
    boost::split(startingDeckSections, startingDecksLines, [](const auto& line) {return line.empty(); });

    Deck deckOne = createDeck(startingDeckSections.at(0));
    Deck deckTwo = createDeck(startingDeckSections.at(1));

    return {deckOne, deckTwo};

}

int winningPlayerScore(const std::vector<std::string>& startingDecksLines)
{
    DeckPair startingDeckPair = parseStartingDecksLines(startingDecksLines);
    CombatCardGameSimulator simulator{std::move(startingDeckPair)};
    Player winningPlayer = simulator.determineWinningPlayer();
    return simulator.getPlayerScore(winningPlayer);
}

int winningPlayerScoreRecursiveCombat(const std::vector<std::string>& startingDecksLines)
{
    DeckPair startingDeckPair = parseStartingDecksLines(startingDecksLines);
    RecursiveCombatCardGameSimulator recursiveSimulator{std::move(startingDeckPair)};
    Player winningPlayer = recursiveSimulator.determineWinningPlayer();
    return recursiveSimulator.getPlayerScore(winningPlayer);
}

}
