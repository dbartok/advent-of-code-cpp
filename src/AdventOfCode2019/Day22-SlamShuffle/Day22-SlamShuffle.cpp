#include "Day22-SlamShuffle.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

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

void applyShuffleInstruction(const ShuffleInstruction& shuffleInstruction, std::vector<unsigned>& cards)
{
    if (shuffleInstruction.type == DEAL_INTO_NEW_STACK)
    {
        std::reverse(cards.begin(), cards.end());
    }
    else if (shuffleInstruction.type == CUT)
    {
        auto copy{cards};
        int numCardsToCut = shuffleInstruction.arg;
        if (numCardsToCut < 0)
        {
            numCardsToCut += cards.size();
        }
        for (size_t i = 0; i < cards.size(); ++i)
        {
            cards[i] = copy[(i + numCardsToCut) % copy.size()];
        }
    }
    else
    {
        auto copy{cards};
        size_t i = 0;
        for (size_t copyIndex = 0; copyIndex < cards.size(); ++copyIndex)
        {
            cards[i] = copy[copyIndex];

            i = (i + shuffleInstruction.arg) % cards.size();
        }
    }
}

void shuffle(const std::vector<ShuffleInstruction>& shuffleInstructions, std::vector<unsigned>& cards)
{
    for (const auto& instruction : shuffleInstructions)
    {
        applyShuffleInstruction(instruction, cards);
    }
}

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

    std::vector<unsigned> cards(deckSize);
    std::iota(cards.begin(), cards.end(), 0);

    shuffle(shuffleInstructions, cards);

    return cards;
}

}
