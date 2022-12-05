#include "Day05-SupplyStacks.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <stack>
#include <regex>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2022
{
namespace Day05
{

struct MoveInstruction
{
    size_t from_index;
    size_t to_index;
    size_t quantity;
};


using CrateStack = std::stack<char>;

class CargoCrane
{
public:
    CargoCrane(std::vector<CrateStack> crateStacks, std::vector<MoveInstruction> moveInstructions)
        : m_crateStacks{std::move(crateStacks)}
        , m_moveInstructions{std::move(moveInstructions)}
    {

    }

    void executeMoveInstructions()
    {
        for (const auto& moveInstruction : m_moveInstructions)
        {
            executeMoveInstruction(moveInstruction);
        }
    }

    std::string getCratesOnTopOfEachStack() const
    {
        std::string cratesOnTopOfEachStack;

        for (const auto& crateStack : m_crateStacks)
        {
            cratesOnTopOfEachStack += crateStack.top();
        }

        return cratesOnTopOfEachStack;
    }

protected:
    std::vector<CrateStack> m_crateStacks;
    std::vector<MoveInstruction> m_moveInstructions;

    virtual void executeMoveInstruction(const MoveInstruction& moveInstruction) = 0;
};

class SingleCrateMovingCargoCrane : public CargoCrane
{
public:
    using CargoCrane::CargoCrane;

protected:
    void executeMoveInstruction(const MoveInstruction& moveInstruction) override
    {
        for (size_t iteration = 0; iteration < moveInstruction.quantity; ++iteration)
        {
            moveSingleCrate(moveInstruction.from_index, moveInstruction.to_index);
        }
    }

private:
    void moveSingleCrate(size_t from_index, size_t to_index)
    {
        const char crate = m_crateStacks.at(from_index).top();
        m_crateStacks.at(from_index).pop();
        m_crateStacks.at(to_index).push(crate);
    }
};

class MultiCrateMovingCargoCrane : public CargoCrane
{
public:
    using CargoCrane::CargoCrane;

protected:
    void executeMoveInstruction(const MoveInstruction& moveInstruction) override
    {
        std::vector<char> cratesToMove;

        for (size_t iteration = 0; iteration < moveInstruction.quantity; ++iteration)
        {
            const char crate = m_crateStacks.at(moveInstruction.from_index).top();
            m_crateStacks.at(moveInstruction.from_index).pop();
            cratesToMove.push_back(crate);
        }

        std::reverse(cratesToMove.begin(), cratesToMove.end());

        for (size_t iteration = 0; iteration < moveInstruction.quantity; ++iteration)
        {
            const char crate = cratesToMove.at(iteration);
            m_crateStacks.at(moveInstruction.to_index).push(crate);
        }
    }

private:
};

using CargoCraneInputTextSection = std::vector<std::string>;

const int FIRST_STACK_COLUMN_INDEX = 1;
const int STACK_STEPS_SIZE = 4;
const int LAST_STACK_ROW_OFFSET = 1;

std::vector<CrateStack> parseStartingStacksSection(const CargoCraneInputTextSection& cargoCraneInputTextSection)
{
    std::vector<CrateStack> crateStacks;

    for (int i = FIRST_STACK_COLUMN_INDEX; i < cargoCraneInputTextSection.at(0).size(); i += STACK_STEPS_SIZE)
    {
        CrateStack crateStack;
        for (int j = cargoCraneInputTextSection.size() - 1 - LAST_STACK_ROW_OFFSET; j >= 0; --j)
        {
            const char element = cargoCraneInputTextSection.at(j).at(i);
            if (element != ' ')
            {
                crateStack.push(element);
            }
        }
        crateStacks.push_back(std::move(crateStack));
    }

    return crateStacks;
}

MoveInstruction parseMoveInstructionLine(const std::string& moveInstructionLine)
{
    std::regex moveInstructionLineRegex(R"(move ([0-9]+) from ([0-9]+) to ([0-9]+))");
    std::smatch moveInstructionLineMatch;

    std::regex_match(moveInstructionLine, moveInstructionLineMatch, moveInstructionLineRegex);

    size_t quantity = std::stoi(moveInstructionLineMatch[1].str());
    size_t from_index = std::stoi(moveInstructionLineMatch[2].str()) - 1;
    size_t to_index = std::stoi(moveInstructionLineMatch[3].str()) - 1;

    return MoveInstruction{from_index, to_index, quantity};
}

std::vector<MoveInstruction> parseRearrangementProcedureSection(const CargoCraneInputTextSection& rearrangementProcedureSection)
{
    std::vector<MoveInstruction> moveInstructions;

    for (const auto& moveInstructionLine : rearrangementProcedureSection)
    {
        MoveInstruction moveInstruction = parseMoveInstructionLine(moveInstructionLine);
        moveInstructions.push_back(std::move(moveInstruction));
    }

    return moveInstructions;
}

template
<class CargoCraneClass>
CargoCraneClass parseStartingStacksAndRearrangementProcedureLines(const std::vector<std::string>& startingStacksAndRearrangementProcedureLines)
{
    std::vector<CargoCraneInputTextSection> cargoCraneInputTextSections;
    boost::split(cargoCraneInputTextSections, startingStacksAndRearrangementProcedureLines, [](const auto& elem) {return elem.empty(); });

    std::vector<CrateStack> crateStack = parseStartingStacksSection(cargoCraneInputTextSections.at(0));
    std::vector<MoveInstruction> moveInstructions = parseRearrangementProcedureSection(cargoCraneInputTextSections.at(1));

    return CargoCraneClass{crateStack, moveInstructions};
}

std::string cratesOnTopOfEachStack(const std::vector<std::string>& startingStacksAndRearrangementProcedureLines)
{
    SingleCrateMovingCargoCrane cargoCrane = parseStartingStacksAndRearrangementProcedureLines<SingleCrateMovingCargoCrane>(startingStacksAndRearrangementProcedureLines);

    cargoCrane.executeMoveInstructions();

    return cargoCrane.getCratesOnTopOfEachStack();
}

std::string cratesOnTopOfEachStackWithNewerTypeCrane(const std::vector<std::string>& startingStacksAndRearrangementProcedureLines)
{
    MultiCrateMovingCargoCrane cargoCrane = parseStartingStacksAndRearrangementProcedureLines<MultiCrateMovingCargoCrane>(startingStacksAndRearrangementProcedureLines);

    cargoCrane.executeMoveInstructions();

    return cargoCrane.getCratesOnTopOfEachStack();
}

}
}
}