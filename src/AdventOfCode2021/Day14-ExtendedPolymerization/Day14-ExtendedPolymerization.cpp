#include "Day14-ExtendedPolymerization.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

size_t NUM_PAIR_INSERTIONS_PART_ONE = 10;

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day14
{

using TextSection = std::vector<std::string>;
using PolymerPairToInsertedElement = std::unordered_map<std::string, char>;
using PolymerPairToInt = std::unordered_map<std::string, int>;

class PolymerizationEquipment
{
public:
    PolymerizationEquipment(std::string polymerTemplate, PolymerPairToInsertedElement polymerPairToInsertedElement)
        : m_polymerPairToInsertedElement{std::move(polymerPairToInsertedElement)}
    {
        for (auto polymerIter = polymerTemplate.cbegin(); polymerIter < polymerTemplate.cend() - 1; ++polymerIter)
        {
            std::string polymerPair = {*polymerIter, *(std::next(polymerIter))};
            ++m_polymerPairToNumOccurrences[polymerPair];
        }
    }

    void growPolymer(size_t numPairInsertions)
    {
        for (size_t i = 0; i < numPairInsertions; ++i)
        {
            executePairInsertions();
        }
    }

    int getMostAndLeastCommonElementQuantityDifference() const
    {
        auto elementToNumOccurrencesInPairs = getElementToNumOccurrencesInPairs();

        const auto minmaxElementToNumOccurrencesInPairsIter = std::minmax_element(elementToNumOccurrencesInPairs.cbegin(), elementToNumOccurrencesInPairs.cend(), [](const auto& lhs, const auto& rhs)
                                                                                  {
                                                                                      return lhs.second < rhs.second;
                                                                                  });

        const int minPolymerNumOccurrencesInPairs = minmaxElementToNumOccurrencesInPairsIter.first->second;
        const int maxPolymerNumOccurrencesInPairs = minmaxElementToNumOccurrencesInPairsIter.second->second;

        // Account for elements at the beginning and end of the polymer
        const int minPolymerQuantity = std::ceil(static_cast<double>(minPolymerNumOccurrencesInPairs) / 2);
        const int maxPolymerQuantity = std::ceil(static_cast<double>(maxPolymerNumOccurrencesInPairs) / 2);

        return maxPolymerQuantity - minPolymerQuantity;
    }

private:
    PolymerPairToInt m_polymerPairToNumOccurrences;
    PolymerPairToInsertedElement m_polymerPairToInsertedElement;

    void executePairInsertions()
    {
        PolymerPairToInt updatedPolymerPairToNumOccurrences;

        for (const auto& polymerPairAndNumOccurrences : m_polymerPairToNumOccurrences)
        {
            const std::string& polymerPair = polymerPairAndNumOccurrences.first;
            const int numOccurrences = polymerPairAndNumOccurrences.second;

            const char insertedElement = m_polymerPairToInsertedElement.at(polymerPair);

            std::string leftPair{polymerPair.front(), insertedElement};
            std::string rightRight{insertedElement, polymerPair.back()};

            updatedPolymerPairToNumOccurrences[leftPair] += numOccurrences;
            updatedPolymerPairToNumOccurrences[rightRight] += numOccurrences;
        }

        m_polymerPairToNumOccurrences = updatedPolymerPairToNumOccurrences;
    }

    std::unordered_map<char, int> getElementToNumOccurrencesInPairs() const
    {
        std::unordered_map<char, int> elementToNumOccurrencesInPairs;

        for (const auto& polymerPairAndNumOccurrences : m_polymerPairToNumOccurrences)
        {
            const std::string& polymerPair = polymerPairAndNumOccurrences.first;
            const int numOccurrences = polymerPairAndNumOccurrences.second;

            elementToNumOccurrencesInPairs[polymerPair.front()] += numOccurrences;
            elementToNumOccurrencesInPairs[polymerPair.back()] += numOccurrences;
        }

        return elementToNumOccurrencesInPairs;
    }
};

PolymerPairToInsertedElement parsePairInsertionRulesTextSection(const TextSection& pairInsertionRulesTextSection)
{
    PolymerPairToInsertedElement polymerPairToInsertedElement;

    for (const auto& line : pairInsertionRulesTextSection)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(" ->"), boost::token_compress_on);
        polymerPairToInsertedElement[tokens.at(0)] = tokens.at(1).front();
    }

    return polymerPairToInsertedElement;
}

PolymerizationEquipment parsePolymerFormulaLines(const std::vector<std::string>& polymerFormulaLines)
{
    std::vector<TextSection> textSections;
    boost::split(textSections, polymerFormulaLines, [](const auto& elem) { return elem.empty(); });

    std::string polymerTemplate = textSections.at(0).at(0);
    PolymerPairToInsertedElement polymerPairToInsertedElement = parsePairInsertionRulesTextSection(textSections.at(1));

    return PolymerizationEquipment{std::move(polymerTemplate), std::move(polymerPairToInsertedElement)};
}

int mostAndLeastCommonElementQuantityDifference(const std::vector<std::string>& polymerFormulaLines)
{
    PolymerizationEquipment polymerizationEquipment = parsePolymerFormulaLines(polymerFormulaLines);
    polymerizationEquipment.growPolymer(NUM_PAIR_INSERTIONS_PART_ONE);
    return polymerizationEquipment.getMostAndLeastCommonElementQuantityDifference();
}

}
}
}