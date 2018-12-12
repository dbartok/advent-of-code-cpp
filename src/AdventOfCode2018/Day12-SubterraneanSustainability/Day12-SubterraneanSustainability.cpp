#include "Day12-SubterraneanSustainability.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <unordered_set>
#include <array>
#include <algorithm>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
const char PLANT_IDENTIFIER = '#';
const unsigned NUM_GROWTH_CYCLES = 20;
}

namespace AdventOfCode
{

using Pots = std::unordered_set<int>;
using NeighborPattern = std::array<bool, 5>;

class PlantNeighborSimulator
{
public:
    PlantNeighborSimulator(Pots initialPotsWithPlants, std::vector<NeighborPattern> neighborPatternsResultingInPlant)
        : m_potsWithPlants{std::move(initialPotsWithPlants)}
        , m_neighborPatternsResultingInPlant{std::move(neighborPatternsResultingInPlant)}
    {

    }

    void simulateGrowth()
    {
        for (unsigned i = 0; i < NUM_GROWTH_CYCLES; ++i)
        {
            executeGrowthCycle();
        }
    }

    int getSumOfPotNumbersWithPlant() const
    {
        return std::accumulate(m_potsWithPlants.cbegin(), m_potsWithPlants.cend(), 0);
    }

private:
    Pots m_potsWithPlants;
    std::vector<NeighborPattern> m_neighborPatternsResultingInPlant;

    void executeGrowthCycle()
    {
        auto minMaxPotsWithPlants = std::minmax_element(m_potsWithPlants.cbegin(), m_potsWithPlants.cend());

        int simulationBegin = *minMaxPotsWithPlants.first - 2;
        int simulationEnd = *minMaxPotsWithPlants.second + 3;

        executeGrowthCycleInPotRange(simulationBegin, simulationEnd);
    }

    void executeGrowthCycleInPotRange(int begin, int end)
    {
        Pots newPotsWithPlants;

        for (int i = begin; i < end; ++i)
        {
            if (isPlantInNextCycle(i))
            {
                newPotsWithPlants.insert(i);
            }
        }

        m_potsWithPlants = newPotsWithPlants;
    }

    bool isPlantInNextCycle(int potNumber) const
    {
        NeighborPattern neighborPattern = getNeighborPattern(potNumber);

        auto findResult = std::find(m_neighborPatternsResultingInPlant.cbegin(), m_neighborPatternsResultingInPlant.cend(), neighborPattern);

        return findResult != m_neighborPatternsResultingInPlant.cend();
    }

    NeighborPattern getNeighborPattern(int potNumber) const
    {
        NeighborPattern neighborPattern;

        size_t neighborPatternIndex = 0;
        for (int i = potNumber - 2; i < potNumber + 3; ++i)
        {
            bool isPlant = (m_potsWithPlants.find(i) != m_potsWithPlants.end());

            neighborPattern[neighborPatternIndex++] = isPlant;
        }

        return neighborPattern;
    }
};

Pots parseInitialPlants(const std::string& initialStateLine)
{
    Pots initialPotsWithPlants;

    std::vector<std::string> tokens;
    boost::split(tokens, initialStateLine, boost::is_any_of(":"), boost::token_compress_on);

    boost::trim(tokens[1]);
    const std::string& initialPotsWithPlantsString = tokens[1];

    for (size_t i = 0; i < initialPotsWithPlantsString.length(); ++i)
    {
        if (initialPotsWithPlantsString[i] == PLANT_IDENTIFIER)
        {
            initialPotsWithPlants.insert(i);
        }
    }

    return initialPotsWithPlants;
}

NeighborPattern parseNeighborPattern(const std::string& neighborPatternString)
{
    NeighborPattern neighborPattern;

    for (size_t i = 0; i < neighborPatternString.length(); ++i)
    {
        const bool isPlant = (neighborPatternString[i] == '#');
        neighborPattern[i] = isPlant;
    }

    return neighborPattern;
}

std::vector<NeighborPattern> parseNeighborPatternsResultingInPlant(const std::vector<std::string>& neighborPatternLines)
{
    std::vector<NeighborPattern> neighborPatternsResultingInPlant;

    for (const auto& lines : neighborPatternLines)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, lines, boost::is_any_of(" =>"), boost::token_compress_on);

        const bool isResultPlant = (tokens[1].front() == PLANT_IDENTIFIER);
        if (isResultPlant)
        {
            neighborPatternsResultingInPlant.push_back(parseNeighborPattern(tokens[0]));
        }
    }

    return neighborPatternsResultingInPlant;
}

int sumOfPotNumbersWithPlantAfterGrowth(const std::string& initialStateLine, const std::vector<std::string>& neighborPatternLines)
{
    Pots initialPotsWithPlants = parseInitialPlants(initialStateLine);
    std::vector<NeighborPattern> neighborPatterns = parseNeighborPatternsResultingInPlant(neighborPatternLines);

    PlantNeighborSimulator simulator{std::move(initialPotsWithPlants), std::move(neighborPatterns)};

    simulator.simulateGrowth();

    return simulator.getSumOfPotNumbersWithPlant();
}

}
