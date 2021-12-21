#include "Day21-DiracDice.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <array>
#include <unordered_map>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

unsigned NUM_DICE = 3;
unsigned NUM_SPACES = 10;
unsigned WINNING_SCORE_THRESHOLD_PART_ONE = 1000;
unsigned WINNING_SCORE_THRESHOLD_PART_TWO = 21;
unsigned DETERMINISTIC_DIE_NUM_SIDES = 100;
unsigned DIRAC_DIE_NUM_SIDES = 3;

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day21
{

class DeterministicDiceGameSimulator
{
public:
    DeterministicDiceGameSimulator(int playerOneInitialPosition, int playerTwoInitialPosition)
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
        return getCurrentPlayerScore() >= WINNING_SCORE_THRESHOLD_PART_ONE;
    }

    int& getCurrentPlayerPosition()
    {
        return m_playerPositions.at(m_currentPlayerIndex);
    }

    int& getCurrentPlayerScore()
    {
        return const_cast<int&>(const_cast<const DeterministicDiceGameSimulator&>(*this).getCurrentPlayerScore());
    }

    const int& getCurrentPlayerScore() const
    {
        return m_playerScores.at(m_currentPlayerIndex);
    }
};

struct MultiverseGameState
{
    MultiverseGameState(uint64_t numUniverses, int score, int position)
        : numUniverses{numUniverses}
        , score{score}
        , position{position}
    {

    }

    uint64_t numUniverses;
    int score;
    int position;
};

class DiracDicePlayer
{
public:
    DiracDicePlayer(int startingPosition)
    {
        m_multiverseGameStates.emplace_back(1, 0, startingPosition);
    }

    void simulateSingleTurn(uint64_t universeScalingFactorFromOtherPlayer)
    {
        std::vector<MultiverseGameState> newMultiverseGameStates;

        for (const auto& diceRollAndNumUniverses : getDiceRollToNumUniverses())
        {
            for (const auto& multiverseGameState : m_multiverseGameStates)
            {
                MultiverseGameState newGameState{multiverseGameState};
                newGameState.numUniverses *= diceRollAndNumUniverses.second;
                newGameState.position += diceRollAndNumUniverses.first;
                newGameState.position = ((newGameState.position - 1) % NUM_SPACES) + 1;
                newGameState.score += newGameState.position;

                if (newGameState.score >= WINNING_SCORE_THRESHOLD_PART_TWO)
                {
                    m_numUniversesWherePlayerWon += newGameState.numUniverses * universeScalingFactorFromOtherPlayer;
                }
                else
                {
                    newMultiverseGameStates.push_back(std::move(newGameState));
                }
            }
        }

        m_multiverseGameStates = newMultiverseGameStates;
    }

    uint64_t getUniverseScalingFactor() const
    {
        return std::accumulate(m_multiverseGameStates.cbegin(), m_multiverseGameStates.cend(), 0ull, [](const auto& acc, const auto& multiverseGameState)
                               {
                                   return acc + multiverseGameState.numUniverses;
                               });
    }

    uint64_t getTotalNumUniversesWherePlayerWon() const
    {
        return m_numUniversesWherePlayerWon;
    }

private:
    std::vector<MultiverseGameState> m_multiverseGameStates;

    uint64_t m_numUniversesWherePlayerWon = 0;

    static std::unordered_map<int, unsigned> getDiceRollToNumUniverses()
    {
        std::unordered_map<int, unsigned> diceRollToNumUniverses{{0, 1}};

        for (unsigned die = 0; die < NUM_DICE; ++die)
        {
            std::unordered_map<int, unsigned> newDiceRollToNumUniverses;

            for (unsigned side = 1; side <= DIRAC_DIE_NUM_SIDES; ++side)
            {
                for (const auto& diceRollAndNumUniverses : diceRollToNumUniverses)
                {
                    newDiceRollToNumUniverses[diceRollAndNumUniverses.first + side] += diceRollAndNumUniverses.second;
                }
            }

            diceRollToNumUniverses = newDiceRollToNumUniverses;
        }

        return diceRollToNumUniverses;
    }
};

class DiracDiceGameSimulator
{
public:
    DiracDiceGameSimulator(DiracDicePlayer playerOne, DiracDicePlayer playerTwo)
        : m_playerOne{playerOne}
        , m_playerTwo{playerTwo}
    {

    }

    void play()
    {
        uint64_t scalingFactor = 1;
        while (true)
        {
            m_playerOne.simulateSingleTurn(scalingFactor);
            scalingFactor = m_playerOne.getUniverseScalingFactor();
            if (scalingFactor == 0)
            {
                break;
            }

            m_playerTwo.simulateSingleTurn(scalingFactor);
            scalingFactor = m_playerTwo.getUniverseScalingFactor();
            if (scalingFactor == 0)
            {
                break;
            }
        }
    }

    uint64_t getNumUniversesWithMoreWins() const
    {
        return std::max(m_playerOne.getTotalNumUniversesWherePlayerWon(), m_playerTwo.getTotalNumUniversesWherePlayerWon());
    }

private:
    DiracDicePlayer m_playerOne;
    DiracDicePlayer m_playerTwo;
};

int parseStartingPositionLine(const std::string& startingPositionLines)
{
    return startingPositionLines.back() - '0';
}

std::pair<int, int> parseStartingPositionLines(const std::vector<std::string>& startingPositionLines)
{
    const int playerOneStartingPosition = parseStartingPositionLine(startingPositionLines.front());
    const int playerTwoStartingPosition = parseStartingPositionLine(startingPositionLines.back());

    return {playerOneStartingPosition, playerTwoStartingPosition};
}

int losingPlayerScoreTimesNumDiceRollsDeterministicDice(const std::vector<std::string>& startingPositionLines)
{
    const std::pair<int, int> startingPositions = parseStartingPositionLines(startingPositionLines);
    DeterministicDiceGameSimulator simulator{startingPositions.first, startingPositions.second};
    simulator.play();

    return simulator.getLosingPlayerScore() * simulator.getNumDiceRolls();
}

uint64_t numUniversesWithMoreWins(const std::vector<std::string>& startingPositionLines)
{
    const std::pair<int, int> startingPositions = parseStartingPositionLines(startingPositionLines);

    DiracDicePlayer playerOne{startingPositions.first};
    DiracDicePlayer playerTwo{startingPositions.second};

    DiracDiceGameSimulator simulator{{std::move(playerOne)}, {std::move(playerTwo)}};

    simulator.play();

    return simulator.getNumUniversesWithMoreWins();
}

}
}
}