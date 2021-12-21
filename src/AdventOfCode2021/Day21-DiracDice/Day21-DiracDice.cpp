#include "Day21-DiracDice.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <array>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

unsigned NUM_DICE = 3;
unsigned NUM_SPACES = 10;
unsigned WINNING_SCORE_THRESOLD = 1000;
unsigned DETERMINISTIC_DIE_NUM_SIDES = 100;

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day21
{

class DiracDiceGameSimulator
{
public:
    DiracDiceGameSimulator(int playerOneInitialPosition, int playerTwoInitialPosition)
        : m_playerPositions{playerOneInitialPosition, playerTwoInitialPosition}
    {

    }

    void play()
    {
        while (true)
        {
            moveCurrentPlayer();

            if (hasCurrentPlayerWon())
            {
                return;
            }

            endTurn();
        }
    }

    int getLosingPlayerScore() const
    {
        return std::min(m_playerScores.front(), m_playerScores.back());
    }

    int getNumDiceRolls() const
    {
        return m_numDiceRolls;
    }

private:
    std::array<int, 2> m_playerPositions;
    std::array<int, 2> m_playerScores{0, 0};
    int m_currentPlayerIndex = 0;

    int m_deterministicDieValue = 1;
    unsigned m_numDiceRolls = 0;

    int rollDice()
    {
        int diceRollSum = 0;
        
        for (unsigned die = 0; die < NUM_DICE; ++die)
        {
            ++m_numDiceRolls;
            diceRollSum += m_deterministicDieValue;
            ++m_deterministicDieValue;
            m_deterministicDieValue = ((m_deterministicDieValue - 1) % DETERMINISTIC_DIE_NUM_SIDES) + 1;
        }

        return diceRollSum;
    }

    void moveCurrentPlayer()
    {
        int& currentPlayerPosition = getCurrentPlayerPosition();
        currentPlayerPosition += rollDice();
        currentPlayerPosition = ((currentPlayerPosition - 1) % NUM_SPACES) + 1;

        getCurrentPlayerScore() += currentPlayerPosition;
    }

    void endTurn()
    {
        m_currentPlayerIndex = 1 - m_currentPlayerIndex;
    }

    bool hasCurrentPlayerWon() const
    {
        return getCurrentPlayerScore() >= WINNING_SCORE_THRESOLD;
    }

    int& getCurrentPlayerPosition()
    {
        return m_playerPositions.at(m_currentPlayerIndex);
    }

    int& getCurrentPlayerScore()
    {
        return const_cast<int&>(const_cast<const DiracDiceGameSimulator&>(*this).getCurrentPlayerScore());
    }

    const int& getCurrentPlayerScore() const
    {
        return m_playerScores.at(m_currentPlayerIndex);
    }
};

int parseStartingPositionLine(const std::string& startingPositionLines)
{
    return startingPositionLines.back() - '0';
}

DiracDiceGameSimulator parseStartingPositionLines(const std::vector<std::string>& startingPositionLines)
{
    const int playerOneStartingPosition = parseStartingPositionLine(startingPositionLines.front());
    const int playerTwoStartingPosition = parseStartingPositionLine(startingPositionLines.back());

    return DiracDiceGameSimulator{playerOneStartingPosition, playerTwoStartingPosition};
}

int losingPlayerScoreTimesNumDiceRollsDeterministicDice(const std::vector<std::string>& startingPositionLines)
{
    DiracDiceGameSimulator simulator = parseStartingPositionLines(startingPositionLines);
    simulator.play();

    return simulator.getLosingPlayerScore() * simulator.getNumDiceRolls();
}

}
}
}