#include "Day24-PlanetOfDiscord.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <algorithm>
#include <cmath>
#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
size_t GRID_SIZE = 5;
}

namespace AdventOfCode
{

using Coordinates = std::pair<int, int>;
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

    int getCurrentBiodiversity() const
    {
        int biodiversity = 0;

        for (int j = 0; j < m_size; ++j)
        {
            for (int i = 0; i < m_size; ++i)
            {
                if (m_currentLayout.find({i, j}) != m_currentLayout.cend())
                {
                    biodiversity += std::pow(2, j * m_size + i);
                }
            }
        }

        return biodiversity;
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
                if (isBugInNextIteration({i, j}))
                {
                    nextLayout.emplace(i, j);
                }
            }
        }

        m_currentLayout = nextLayout;
    }

    unsigned getNumNeighborsAt(const Coordinates& coordinates) const
    {
        auto neighborCoordinates = getNeighborCoordinates(coordinates);
        return std::count_if(neighborCoordinates.cbegin(), neighborCoordinates.cend(), [this](const auto& coords)
                             {
                                 return this->m_currentLayout.find(coords) != this->m_currentLayout.cend();
                             });
    }

    std::vector<Coordinates> getNeighborCoordinates(const Coordinates& coordinates) const
    {
        std::vector<Coordinates> neighbors;
        neighbors.push_back({coordinates.first + 1, coordinates.second});
        neighbors.push_back({coordinates.first - 1, coordinates.second});
        neighbors.push_back({coordinates.first, coordinates.second + 1});
        neighbors.push_back({coordinates.first, coordinates.second - 1});
        return neighbors;
    }

    bool isBugInNextIteration(const Coordinates& coordinates) const
    {
        bool isBug = m_currentLayout.find(coordinates) != m_currentLayout.cend();
        int numNeighbors = getNumNeighborsAt(coordinates);
        if (isBug)
        {
            return numNeighbors == 1;
        }
        else
        {
            return numNeighbors == 1 || numNeighbors == 2;
        }
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
                layout.emplace(i, j);
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

}
