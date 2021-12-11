#include "Day11-DumboOctopus.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <algorithm>
#include <iterator>
#include <unordered_set>

#include <boost/functional/hash.hpp>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const size_t NUM_STEPS = 100;
const int FLASH_ENERGY_LEVEL_THRESOLD = 10;

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day11
{

using EnergyLevelRow = std::vector<int>;
using EnergyLevelGrid = std::vector<EnergyLevelRow>;

using Coordinates = std::pair<int, int>;
using CoordinatesSet = std::unordered_set<Coordinates, boost::hash<Coordinates>>;

class OctopusGridSimulator
{
public:
    OctopusGridSimulator(EnergyLevelGrid initialEnergyLevelGrid)
        : m_energyLevelGrid{std::move(initialEnergyLevelGrid)}
        , m_width{m_energyLevelGrid.at(0).size()}
        , m_height{m_energyLevelGrid.size()}
    {

    }

    void simulate(size_t numSteps)
    {
        for (size_t step = 0; step < numSteps; ++step)
        {
            simulateDay();
        }
    }

    void simulateUntilSynchronizedFlash()
    {
        unsigned allPreviousStepsNumFlashes = 0;

        while (true)
        {
            simulateDay();

            const unsigned currentStepNumFlashes = m_numTotalFlashes - allPreviousStepsNumFlashes;
            if (currentStepNumFlashes == m_height * m_width)
            {
                break;
            }

            allPreviousStepsNumFlashes += currentStepNumFlashes;
         }
    }

    unsigned getNumTotalFlashes()
    {
        return m_numTotalFlashes;
    }

    unsigned getNumStepsElapsed()
    {
        return m_numStepsElapsed;
    }

private:
    EnergyLevelGrid m_energyLevelGrid;

    const size_t m_width;
    const size_t m_height;

    unsigned m_numTotalFlashes = 0;
    unsigned m_numStepsElapsed = 0;

    void simulateDay()
    {
        ++m_numStepsElapsed;

        for (size_t j = 0; j < m_height; ++j)
        {
            for (size_t i = 0; i < m_width; ++i)
            {
                increaseEnergyLevelAt({i, j});
            }
        }

        for (size_t j = 0; j < m_height; ++j)
        {
            for (size_t i = 0; i < m_width; ++i)
            {
                resetIfFlashedAt({i, j});
            }
        }
    }

    void increaseEnergyLevelAt(const Coordinates& coordinates)
    {
        const int newEnergyLevel = ++energyLevelAt(coordinates);

        if (newEnergyLevel == FLASH_ENERGY_LEVEL_THRESOLD)
        {
            increaseEnergyAtAdjacentCoordinates(coordinates);
            ++m_numTotalFlashes;
        }
    }

    int& energyLevelAt(const Coordinates& coordinates)
    {
        return m_energyLevelGrid.at(coordinates.second).at(coordinates.first);
    }

    void increaseEnergyAtAdjacentCoordinates(const Coordinates& baseCoordinates)
    {
        const std::vector<Coordinates> allAdjacentCoordinates = getAllAdjacentCoordinates(baseCoordinates);

        for (const auto& coordinates : allAdjacentCoordinates)
        {
            increaseEnergyLevelAt(coordinates);
        }
    }

    std::vector<Coordinates> getAllAdjacentCoordinates(const Coordinates& coordinates) const
    {
        const size_t yMin = std::max(0, coordinates.second - 1);
        const size_t yMax = std::min(static_cast<int>(m_height) - 1, coordinates.second + 1);

        const size_t xMin = std::max(0, coordinates.first - 1);
        const size_t xMax = std::min(static_cast<int>(m_width) - 1, coordinates.first + 1);

        std::vector<Coordinates> allAdjacentCoordinates;

        for (size_t j = yMin; j <= yMax; ++j)
        {
            for (size_t i = xMin; i <= xMax; ++i)
            {
                if (!(i == coordinates.first && j == coordinates.second))
                {
                    allAdjacentCoordinates.emplace_back(i, j);
                }
            }
        }

        return allAdjacentCoordinates;
    }

    void resetIfFlashedAt(const Coordinates& coordinates)
    {
        int& energyLevel = energyLevelAt(coordinates);
        if (energyLevel >= FLASH_ENERGY_LEVEL_THRESOLD)
        {
            energyLevel = 0;
        }
    }
};


EnergyLevelRow parseEnergyLevelLine(const std::string& energyLevelLine)
{
    EnergyLevelRow energyLevelRow;

    std::transform(energyLevelLine.cbegin(), energyLevelLine.cend(), std::back_inserter(energyLevelRow), [](const char c)
                   {
                       return c - '0';
                   });

    return energyLevelRow;
}

EnergyLevelGrid parseEnergyLevelLines(const std::vector<std::string>& energyLevelLines)
{
    EnergyLevelGrid energyLevelGrid;

    for (const auto& line : energyLevelLines)
    {
        EnergyLevelRow energyLevelRow = parseEnergyLevelLine(line);
        energyLevelGrid.push_back(std::move(energyLevelRow));
    }

    return energyLevelGrid;
}

unsigned numTotalFlashes(const std::vector<std::string>& initialEnergyLevelLines)
{
    EnergyLevelGrid initialEnergyLevelGrid = parseEnergyLevelLines(initialEnergyLevelLines);
    OctopusGridSimulator octopusGridSimulator{std::move(initialEnergyLevelGrid)};
    octopusGridSimulator.simulate(NUM_STEPS);
    return octopusGridSimulator.getNumTotalFlashes();
}

unsigned numStepsUntilSynchonizedFlash(const std::vector<std::string>& initialEnergyLevelLines)
{
    EnergyLevelGrid initialEnergyLevelGrid = parseEnergyLevelLines(initialEnergyLevelLines);
    OctopusGridSimulator octopusGridSimulator{std::move(initialEnergyLevelGrid)};
    octopusGridSimulator.simulateUntilSynchronizedFlash();
    return octopusGridSimulator.getNumStepsElapsed();
}

}
}
}