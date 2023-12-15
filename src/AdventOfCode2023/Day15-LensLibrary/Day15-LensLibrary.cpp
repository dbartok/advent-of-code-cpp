#include "Day15-LensLibrary.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <vector>
#include <numeric>
#include <array>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2023
{
namespace Day15
{

class InitializationSequenceAnalyzer
{
public:
    InitializationSequenceAnalyzer(std::vector<std::string> initializationSequence)
        : m_initializationSequence{std::move(initializationSequence)}
    {

    }

    void executeSequence()
    {
        for (const auto& instruction : m_initializationSequence)
        {
            executeInstruction(instruction);
        }
    }

    int getSumOfInitializationSequenceHashes() const
    {
        return std::accumulate(m_initializationSequence.cbegin(), m_initializationSequence.cend(), 0, [](int acc, const auto& initializationString)
                               {
                                   return acc + getHashValue(initializationString);
                               });
    }

    int getFocusingPower() const
    {
        int focusingPower = 0;

        for (int boxIndex = 0; boxIndex < m_boxes.size(); ++boxIndex)
        {
            for (int slotIndex = 0; slotIndex < m_boxes.at(boxIndex).size(); ++slotIndex)
            {
                focusingPower +=(boxIndex + 1) * (slotIndex + 1) * m_boxes.at(boxIndex).at(slotIndex).focalLength;
            }
        }

        return focusingPower;
    }

private:
    struct Lens
    {
        std::string label;
        int focalLength;

        Lens(std::string label, int focalLength)
            : label{std::move(label)}
            , focalLength(focalLength)
        {

        }
    };

    using Box = std::vector<Lens>;
    using Boxes = std::array<Box, 256>;

    std::vector<std::string> m_initializationSequence;

    Boxes m_boxes;

    void executeInstruction(const std::string& instruction)
    {
        if (instruction.back() == '-')
        {
            executeRemoval(instruction);
        }
        else
        {
            executeUpsertion(instruction);
        }
    }

    void executeRemoval(const std::string& instruction)
    {
        const std::string label = instruction.substr(0, instruction.size() - 1);
        const size_t boxIndex = getHashValue(label);
        Box& box = m_boxes.at(boxIndex);

        box.erase(std::remove_if(box.begin(), box.end(), [&label](const auto& lens) { return lens.label == label; }), box.end());
    }

    void executeUpsertion(const std::string& instruction)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, instruction, boost::is_any_of("="));

        const std::string label = tokens.front();
        const size_t boxIndex = getHashValue(label);
        Box& box = m_boxes.at(boxIndex);
        const int focalLength = std::stoi(tokens.back());

        auto findResult = std::find_if(box.begin(), box.end(), [&label](const auto& lens) { return lens.label == label; });
        if (findResult == box.cend())
        {
            box.emplace_back(std::move(label), focalLength);
        }
        else
        {
            findResult->focalLength = focalLength;
        }
    }

    static int getHashValue(const std::string& str)
    {
        int hashValue = 0;

        for (const char c : str)
        {
            hashValue += c;
            hashValue *= 17;
            hashValue %= 256;
        }

        return hashValue;
    }
};

std::vector<std::string> parseInitializationSequenceString(const std::string& initializationSequenceString)
{
    std::vector<std::string> initializationSequence;
    boost::split(initializationSequence, initializationSequenceString, boost::is_any_of(","));
    return initializationSequence;
}

int sumOfHashResults(const std::string& initializationSequenceString)
{
    std::vector<std::string> initializationSequence = parseInitializationSequenceString(initializationSequenceString);

    InitializationSequenceAnalyzer initializationSequenceAnalyzer{std::move(initializationSequence)};

    return initializationSequenceAnalyzer.getSumOfInitializationSequenceHashes();
}

int focusingPowerOfLensConfiguration(const std::string& initializationSequenceString)
{
    std::vector<std::string> initializationSequence = parseInitializationSequenceString(initializationSequenceString);

    InitializationSequenceAnalyzer initializationSequenceAnalyzer{std::move(initializationSequence)};

    initializationSequenceAnalyzer.executeSequence();

    return initializationSequenceAnalyzer.getFocusingPower();
}

}
}
}