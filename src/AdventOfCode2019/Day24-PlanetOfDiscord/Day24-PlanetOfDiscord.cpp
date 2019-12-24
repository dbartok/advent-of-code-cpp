#include "Day24-PlanetOfDiscord.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <algorithm>
#include <cmath>
#include <tuple>
#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
size_t GRID_SIZE = 5;
}

namespace AdventOfCode
{

using Coordinates = std::tuple<int, int, int>;
using Layout = std::unordered_set<Coordinates, boost::hash<Coordinates>>;
using Layouts = std::vector<Layout>;

class BugCellularAutomaton
{
public:
    BugCellularAutomaton(Layout initialLayout, size_t size)
        : m_currentLayout{std::move(initialLayout)}
        , m_size{size}
    {

    }

    void simulateUntilFirstDuplicate()
    {
        while (std::find(m_previousLayouts.cbegin(), m_previousLayouts.cend(), m_currentLayout) == m_previousLayouts.cend())
        {
            m_previousLayouts.push_back(m_currentLayout);
            executeSimulationStep();
        }
    }

    void simulateStepsRecursiveGrid(unsigned numSteps)
    {
        for (size_t i = 0; i < numSteps; ++i)
        {
            executeSimulationStepRecursiveGrid();
        }
    }

    int getCurrentBiodiversity() const
    {
        int biodiversity = 0;

        for (int j = 0; j < m_size; ++j)
        {
            for (int i = 0; i < m_size; ++i)
            {
                if (m_currentLayout.find({0, i, j}) != m_currentLayout.cend())
                {
                    biodiversity += std::pow(2, j * m_size + i);
                }
            }
        }

        return biodiversity;
    }

    unsigned getNumBugs()
    {
        return m_currentLayout.size();
    }

private:
    Layout m_currentLayout;
    size_t m_size;

    Layouts m_previousLayouts;

    void executeSimulationStep()
    {
        Layout nextLayout;

        for (int j = 0; j < m_size; ++j)
        {
            for (int i = 0; i < m_size; ++i)
            {
                Coordinates coordinates{0, i, j};
                int numSurroundingBugs = getNumSurroundingBugs(coordinates);
                if (isBugInNextIteration(coordinates, numSurroundingBugs))
                {
                    nextLayout.insert(std::move(coordinates));
                }
            }
        }

        m_currentLayout = nextLayout;
    }

    void executeSimulationStepRecursiveGrid()
    {
        Layout nextLayout;

        std::pair<int, int> levelBounds = getLevelBounds();

        for (int level = levelBounds.first - 1; level <= levelBounds.second + 1; ++level)
        {
            for (int j = 0; j < m_size; ++j)
            {
                for (int i = 0; i < m_size; ++i)
                {
                    if (i == 2 && j == 2)
                    {
                        continue;
                    }

                    Coordinates coordinates{level, i, j};
                    int numSurroundingBugs = getNumSurroundingBugsRecursiveAt(coordinates);
                    if (isBugInNextIteration(coordinates, numSurroundingBugs))
                    {
                        nextLayout.insert(std::move(coordinates));
                    }
                }
            }
        }

        m_currentLayout = nextLayout;
    }

    unsigned getNumSurroundingBugs(const Coordinates& coordinates) const
    {
        auto neighborCoordinates = getSameLevelNeighborCoordinates(coordinates);
        return numberOfBugs(neighborCoordinates);
    }

    unsigned getNumSurroundingBugsRecursiveAt(const Coordinates& coordinates) const
    {
        std::vector<Coordinates> allNeighborCoordinates = getSameLevelNeighborCoordinates(coordinates);

        std::vector<Coordinates> nextLevelNeighborCoordinates = getNextLevelNeighborCoordinates(coordinates);
        allNeighborCoordinates.insert(allNeighborCoordinates.end(), nextLevelNeighborCoordinates.begin(), nextLevelNeighborCoordinates.end());

        std::vector<Coordinates> prevLevelNeighborCoordinates = getPrevLevelNeighborCoordinates(coordinates);
        allNeighborCoordinates.insert(allNeighborCoordinates.end(), prevLevelNeighborCoordinates.begin(), prevLevelNeighborCoordinates.end());

        return numberOfBugs(allNeighborCoordinates);
    }

    std::vector<Coordinates> getSameLevelNeighborCoordinates(const Coordinates& coordinates) const
    {
        std::vector<Coordinates> neighbors;
        int level, x, y;
        std::tie(level, x, y) = coordinates;
        neighbors.emplace_back(level, x + 1, y);
        neighbors.emplace_back(level, x - 1, y);
        neighbors.emplace_back(level, x, y + 1);
        neighbors.emplace_back(level, x, y - 1);
        return neighbors;
    }

    std::vector<Coordinates> getNextLevelNeighborCoordinates(const Coordinates& coordinates) const
    {
        std::vector<Coordinates> neighbors;
        int level, x, y;
        std::tie(level, x, y) = coordinates;

        if (x == 2 && y == 1)
        {
            for (int i = 0; i < GRID_SIZE; ++i)
            {
                neighbors.emplace_back(level + 1, i, 0);
            }
        }

        if (x == 2 && y == 3)
        {
            for (int i = 0; i < GRID_SIZE; ++i)
            {
                neighbors.emplace_back(level + 1, i, GRID_SIZE - 1);
            }
        }

        if (x == 1 && y == 2)
        {
            for (int j = 0; j < GRID_SIZE; ++j)
            {
                neighbors.emplace_back(level + 1, 0, j);
            }
        }

        if (x == 3 && y == 2)
        {
            for (int j = 0; j < GRID_SIZE; ++j)
            {
                neighbors.emplace_back(level + 1, GRID_SIZE - 1, j);
            }
        }

        return neighbors;
    }

    std::vector<Coordinates> getPrevLevelNeighborCoordinates(const Coordinates& coordinates) const
    {
        std::vector<Coordinates> neighbors;
        int level, x, y;
        std::tie(level, x, y) = coordinates;

        if (y == 0)
        {
            neighbors.emplace_back(level - 1, 2, 1);
        }

        if (y == GRID_SIZE - 1)
        {
            neighbors.emplace_back(level - 1, 2, 3);
        }

        if (x == 0)
        {
            neighbors.emplace_back(level - 1, 1, 2);
        }

        if (x == GRID_SIZE - 1)
        {
            neighbors.emplace_back(level - 1, 3, 2);
        }

        return neighbors;
    }

    unsigned numberOfBugs(const std::vector<Coordinates>& possibleCoordinates) const
    {
        return std::count_if(possibleCoordinates.cbegin(), possibleCoordinates.cend(), [this](const auto& coords)
                             {
                                 return this->m_currentLayout.find(coords) != this->m_currentLayout.cend();
                             });
    }

    bool isBugInNextIteration(const Coordinates& coordinates, unsigned numSurroundingBugs) const
    {
        bool isBug = m_currentLayout.find(coordinates) != m_currentLayout.cend();
        if (isBug)
        {
            return numSurroundingBugs == 1;
        }
        else
        {
            return numSurroundingBugs == 1 || numSurroundingBugs == 2;
        }
    }

    std::pair<int, int> getLevelBounds() const
    {
        auto minmaxLevelCoordsIterPair = std::minmax_element(m_currentLayout.cbegin(), m_currentLayout.cend(), [](const auto& lhs, const auto& rhs)
                                                             {
                                                                 return std::get<0>(lhs) < std::get<0>(rhs);
                                                             });

        const auto& minLevelCoords = *minmaxLevelCoordsIterPair.first;
        const auto& maxLevelCoords = *minmaxLevelCoordsIterPair.second;
        const int minLevel = std::get<0>(minLevelCoords);
        const int maxLevel = std::get<0>(maxLevelCoords);

        return {minLevel, maxLevel};
    }
};

Layout createLayout(const std::vector<std::string>& layoutLines)
{
    Layout layout;

    for (int j = 0; j < layoutLines.size(); ++j)
    {
        for (int i = 0; i < layoutLines.at(j).size(); ++i)
        {
            if (layoutLines.at(j).at(i) == '#')
            {
                layout.emplace(0, i, j);
            }
        }
    }

    return layout;
}

int biodiversityRatingOfFirstDuplicateLayout(const std::vector<std::string>& initialLayoutLines)
{
    Layout initialLayout = createLayout(initialLayoutLines);

    BugCellularAutomaton automaton{initialLayout, GRID_SIZE};

    automaton.simulateUntilFirstDuplicate();

    return automaton.getCurrentBiodiversity();
}

unsigned numBugsAfterSimulationRecursiveGrid(const std::vector<std::string>& initialLayoutLines, unsigned numSteps)
{
    Layout initialLayout = createLayout(initialLayoutLines);

    BugCellularAutomaton automaton{initialLayout, GRID_SIZE};

    automaton.simulateStepsRecursiveGrid(numSteps);

    return automaton.getNumBugs();
}

}
