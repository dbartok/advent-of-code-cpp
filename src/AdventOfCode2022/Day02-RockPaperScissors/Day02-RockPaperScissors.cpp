#include "Day02-RockPaperScissors.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <stdexcept>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const int NUM_SHAPES = 3;
const char PLAYER_ROCK = 'X';
const char PLAYER_PAPER = 'Y';
const char PLAYER_SCISSORS = 'Z';
const char OPPONENT_ROCK = 'A';
const char OPPONENT_PAPER = 'B';
const char OPPONENT_SCISSORS = 'C';
const int SCORE_ROCK = 1;
const int SCORE_PAPER = 2;
const int SCORE_SCISSORS = 3;
const int SCORE_WIN = 6;
const int SCORE_DRAW = 3;
const int SCORE_LOSS = 0;

}

namespace AdventOfCode
{
namespace Year2022
{
namespace Day02
{

enum class ROCK_PAPER_SCISSORS_HAND
{
    ROCK = 0,
    PAPER = 1,
    SCISSORS = 2,
};

enum class ROCK_PAPER_SCISSORS_ROUND_OUTCOME
{
    WIN,
    DRAW,
    LOSS,
};

class RockPaperScissorsRound
{
public:
    RockPaperScissorsRound(ROCK_PAPER_SCISSORS_HAND playerHand, ROCK_PAPER_SCISSORS_HAND opponentHand)
        : m_playerHand(playerHand)
        , m_opponentHand(opponentHand)
    {

    }

    int getPlayerScore() const
    {
        auto s = getSelectedShapeScore() + getOutcomeScore();
        return s;
    }

private:
    int getSelectedShapeScore() const
    {
        switch (m_playerHand)
        {
            case ROCK_PAPER_SCISSORS_HAND::ROCK:
                return SCORE_ROCK;
            case ROCK_PAPER_SCISSORS_HAND::PAPER:
                return SCORE_PAPER;
            case ROCK_PAPER_SCISSORS_HAND::SCISSORS:
                return SCORE_SCISSORS;
            default:
                throw std::runtime_error("Invalid player hand: " + std::to_string(static_cast<int>(m_playerHand)));
        }
    }

    int getOutcomeScore() const
    {
        const ROCK_PAPER_SCISSORS_ROUND_OUTCOME outcome = getOutcome();

        switch (outcome)
        {
            case ROCK_PAPER_SCISSORS_ROUND_OUTCOME::LOSS:
                return SCORE_LOSS;
            case ROCK_PAPER_SCISSORS_ROUND_OUTCOME::DRAW:
                return SCORE_DRAW;
            case ROCK_PAPER_SCISSORS_ROUND_OUTCOME::WIN:
                return SCORE_WIN;
            default:
                throw std::runtime_error("Invalid outcome: " + std::to_string(static_cast<int>(outcome)));
        }
    }

    ROCK_PAPER_SCISSORS_ROUND_OUTCOME getOutcome() const
    {
        const ROCK_PAPER_SCISSORS_HAND handThatBeatsPlayerHand = static_cast<ROCK_PAPER_SCISSORS_HAND>((static_cast<int>(m_playerHand) + 1) % NUM_SHAPES);

        if (m_opponentHand == handThatBeatsPlayerHand)
        {
            return ROCK_PAPER_SCISSORS_ROUND_OUTCOME::LOSS;
        }

        if (m_opponentHand == m_playerHand)
        {
            return ROCK_PAPER_SCISSORS_ROUND_OUTCOME::DRAW;
        }

        return ROCK_PAPER_SCISSORS_ROUND_OUTCOME::WIN;
    }

    ROCK_PAPER_SCISSORS_HAND m_playerHand;
    ROCK_PAPER_SCISSORS_HAND m_opponentHand;
};

ROCK_PAPER_SCISSORS_HAND parsePlayerHandChar(char playerHandChar)
{
    switch (playerHandChar)
    {
        case PLAYER_ROCK:
            return ROCK_PAPER_SCISSORS_HAND::ROCK;
        case PLAYER_PAPER:
            return ROCK_PAPER_SCISSORS_HAND::PAPER;
        case PLAYER_SCISSORS:
            return ROCK_PAPER_SCISSORS_HAND::SCISSORS;
        default:
            throw std::runtime_error("Invalid player hand character: " + std::string{playerHandChar});
    }
}

ROCK_PAPER_SCISSORS_HAND parseOpponentHandChar(char opponentHandChar)
{
    switch (opponentHandChar)
    {
        case OPPONENT_ROCK:
            return ROCK_PAPER_SCISSORS_HAND::ROCK;
        case OPPONENT_PAPER:
            return ROCK_PAPER_SCISSORS_HAND::PAPER;
        case OPPONENT_SCISSORS:
            return ROCK_PAPER_SCISSORS_HAND::SCISSORS;
        default:
            throw std::runtime_error("Invalid opponent hand character: " + std::string{opponentHandChar});
    }
}


RockPaperScissorsRound parseStrategyGuideLine(const std::string& strategyGuideLine)
{
    ROCK_PAPER_SCISSORS_HAND playerHand = parsePlayerHandChar(strategyGuideLine.at(2));
    ROCK_PAPER_SCISSORS_HAND opponentHand = parseOpponentHandChar(strategyGuideLine.at(0));

    return RockPaperScissorsRound{playerHand, opponentHand};
}

std::vector<RockPaperScissorsRound> parseStrategyGuideLines(const std::vector<std::string>& strategyGuideLines)
{
    std::vector<RockPaperScissorsRound> rockPaperScissorsRounds;

    for (const auto& strategyGuideLine : strategyGuideLines)
    {
        RockPaperScissorsRound rockPaperScissorsRound = parseStrategyGuideLine(strategyGuideLine);
        rockPaperScissorsRounds.push_back(std::move(rockPaperScissorsRound));
    }

    return rockPaperScissorsRounds;
}

int totalScoreAccordingToStrategyGuide(const std::vector<std::string>& strategyGuideLines)
{
    std::vector<RockPaperScissorsRound> rockPaperScissorsRounds = parseStrategyGuideLines(strategyGuideLines);

    return std::accumulate(rockPaperScissorsRounds.cbegin(), rockPaperScissorsRounds.cend(), 0, [](const auto& acc, const auto& round) {return acc + round.getPlayerScore(); });
}

}
}
}