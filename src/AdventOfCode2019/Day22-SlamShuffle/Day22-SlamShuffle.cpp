#include "Day22-SlamShuffle.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/integer/mod_inverse.hpp>

#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

enum ShuffleType
{
    DEAL_INTO_NEW_STACK,
    DEAL_WITH_INCREMENT,
    CUT,
};

struct ShuffleInstruction
{
    ShuffleType type;
    int arg;
};

class SlamShuffler
{
public:
    SlamShuffler(std::vector<ShuffleInstruction> instructions, BigNumber deckSize, BigNumber numIterations)
        : m_instructions{std::move(instructions)}
        , m_deckSize{deckSize}
        , m_numIterations{numIterations}
    {

    }

    BigNumber getCardAtPosition(BigNumber position)
    {
        BigNumber iterOneResult = getWhereCardComesFrom(position);
        BigNumber iterTwoResult = getWhereCardComesFrom(iterOneResult);

        // Subtract the following equations:
        // slope * position + offset = iterOneResult
        // slope * iterOneResult + offset = iterTwoResult
        // -> slope = (iterOneResult - iterTwoResult) / (position - iterOneResult)
        BigNumber slope = (iterOneResult - iterTwoResult) * boost::integer::mod_inverse(position - iterOneResult + m_deckSize, m_deckSize) % m_deckSize;
        BigNumber offset = (iterOneResult - slope * position + m_deckSize * position * slope) % m_deckSize;

        // For 2 iterations:
        // result = slope * (slope * position + offset) + offset
        // For m_numIterations iterations:
        // (slope^m_numIterations) * position + (slope^(m_numIterations - 1))) * offset + (slope^(m_numIterations - 2))) * offset + ... + offset
        // (slope^m_numIterations) * position + (slope^m_numIterations - 1) / (slope - 1) * B
        BigNumber slopeToTheNumberOfIterations = boost::multiprecision::powm(slope, m_numIterations, m_deckSize);
        BigNumber firstTerm = slopeToTheNumberOfIterations  * position;
        BigNumber secondTerm = (slopeToTheNumberOfIterations - 1) * boost::integer::mod_inverse(slope - 1 + m_deckSize, m_deckSize) * offset;

        return (firstTerm + secondTerm) % m_deckSize;
    }

    std::vector<unsigned> getAllCardsAfterSingleIteration()
    {
        std::vector<unsigned> allCards;

        for (int i = 0; i < m_deckSize; ++i)
        {
            auto cardAtPosition = getWhereCardComesFrom(i);
            allCards.push_back(unsigned{cardAtPosition});
        }

        return allCards;
    }

private:
    std::vector<ShuffleInstruction> m_instructions;
    BigNumber m_deckSize;
    BigNumber m_numIterations;

    BigNumber getWhereCardComesFrom(BigNumber position)
    {
        for (auto instructionIter = m_instructions.crbegin(); instructionIter != m_instructions.crend(); ++instructionIter)
        {
            position = executeInReverse(*instructionIter, position);
        }

        return position;
    }

    BigNumber executeInReverse(const ShuffleInstruction& instruction, BigNumber position)
    {
        if (instruction.type == DEAL_INTO_NEW_STACK)
        {
            return m_deckSize - 1 - position;
        }
        else if (instruction.type == CUT)
        {
            return (position + instruction.arg + m_deckSize) % m_deckSize;
        }
        else
        {
            BigNumber inverse = boost::integer::mod_inverse(BigNumber{instruction.arg}, m_deckSize);
            return inverse * position % m_deckSize;
        }
    }
};

std::vector<ShuffleInstruction> createShuffleInstructions(const std::vector<std::string>& shuffleInstructionLines)
{
    std::vector<ShuffleInstruction> shuffleInstructions;

    for (const auto& line : shuffleInstructionLines)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(" "));

        ShuffleInstruction shuffleInstruction;
        if (tokens.back() == "stack")
        {
            shuffleInstruction.type = ShuffleType::DEAL_INTO_NEW_STACK;
        }
        else if (tokens.front() == "cut")
        {
            shuffleInstruction.type = ShuffleType::CUT;
            shuffleInstruction.arg = std::stoi(tokens.back());
        }
        else
        {
            shuffleInstruction.type = ShuffleType::DEAL_WITH_INCREMENT;
            shuffleInstruction.arg = std::stoi(tokens.back());
        }
        shuffleInstructions.push_back(std::move(shuffleInstruction));
    }

    return shuffleInstructions;
}

std::vector<unsigned> cardsAfterShuffle(const std::vector<std::string>& shuffleInstructionLines, unsigned deckSize)
{
    std::vector<ShuffleInstruction> shuffleInstructions = createShuffleInstructions(shuffleInstructionLines);

    SlamShuffler shuffler{shuffleInstructions, deckSize, 1};

    return shuffler.getAllCardsAfterSingleIteration();
}

BigNumber cardAtPositionAfterMultipleShuffles(const std::vector<std::string>& shuffleInstructionLines, BigNumber deckSize, BigNumber numIterations, BigNumber position)
{
    std::vector<ShuffleInstruction> shuffleInstructions = createShuffleInstructions(shuffleInstructionLines);

    SlamShuffler shuffler{shuffleInstructions, deckSize, numIterations};

    return shuffler.getCardAtPosition(position);
}

}
