#include "Day15-Chiton.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <algorithm>
#include <iterator>
#include <set>
#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

size_t SCALE_FACTOR_PART_TWO = 5;

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day15
{

using RiskLevelRow = std::vector<int>;
using RiskLevelMap = std::vector<RiskLevelRow>;

using Coordinates = std::pair<int, int>;

struct Node
{
    Coordinates coordinates;
    int riskLevel;
    int bestPathCostSoFar;
};

struct NodePtrCostCmp
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        if (lhs->bestPathCostSoFar < rhs->bestPathCostSoFar)
        {
            return true;
        }

        if (lhs->bestPathCostSoFar > rhs->bestPathCostSoFar)
        {
            return false;
        }

        return lhs < rhs;
    }
};

class CaveTraverser
{
public:
    CaveTraverser(const RiskLevelMap& riskLevelMap)
        : m_width{riskLevelMap.at(0).size()}
        , m_height(riskLevelMap.size())
    {
        for (int j = 0; j < m_width; ++j)
        {
            for (int i = 0; i < m_height; ++i)
            {
                const Coordinates coordinates{i, j};
                Node node{coordinates, riskLevelMap.at(j).at(i), std::numeric_limits<int>::max()};
                m_coordinatesToNode.insert({std::move(coordinates), std::move(node)});
            }
        }
    }

    void traverse()
    {
        Node& startNode = m_coordinatesToNode.at({0, 0});
        startNode.bestPathCostSoFar = 0;
        m_activeNodePtrs.insert(&startNode);

        while (!m_activeNodePtrs.empty())
        {
            Node* closestNodePtr = *m_activeNodePtrs.begin();
            m_activeNodePtrs.erase(closestNodePtr);

            if (isDestination(closestNodePtr->coordinates))
            {
                m_MinRiskLevel = closestNodePtr->bestPathCostSoFar;
                return;
            }

            for (const auto& neighborCoordinates : getAllNeighborCoordinates(closestNodePtr->coordinates))
            {
                Node& neighborNode = m_coordinatesToNode.at(neighborCoordinates);
                const int pathToNeighborCost = closestNodePtr->bestPathCostSoFar + neighborNode.riskLevel;
                if (pathToNeighborCost < neighborNode.bestPathCostSoFar)
                {
                    m_activeNodePtrs.erase(&neighborNode);
                    neighborNode.bestPathCostSoFar = pathToNeighborCost;
                    m_activeNodePtrs.insert(&neighborNode);
                }
            }
        }
    }

    int getMinRiskLevel() const
    {
        return m_MinRiskLevel;
    }

private:
    std::unordered_map<Coordinates, Node, boost::hash<Coordinates>> m_coordinatesToNode;
    size_t m_width;
    size_t m_height;

    std::set<Node*, NodePtrCostCmp> m_activeNodePtrs;

    int m_MinRiskLevel = std::numeric_limits<int>::max();

    std::vector<Coordinates> getAllNeighborCoordinates(const Coordinates& baseCoordinates) const
    {
        const std::vector<Coordinates> possibleNeighborCoordinates =
        {
            Coordinates{baseCoordinates.first, baseCoordinates.second - 1},
            Coordinates{baseCoordinates.first, baseCoordinates.second + 1},
            Coordinates{baseCoordinates.first - 1, baseCoordinates.second},
            Coordinates{baseCoordinates.first + 1, baseCoordinates.second}
        };

        std::vector<Coordinates> inBoundsNeighborCoordinates;

        std::copy_if(possibleNeighborCoordinates.cbegin(),
                     possibleNeighborCoordinates.cend(),
                     std::back_inserter(inBoundsNeighborCoordinates),
                     [this](const auto& coords)
                     {
                         return this->isInBounds(coords);
                     });

        return inBoundsNeighborCoordinates;
    }

    bool isInBounds(const Coordinates& coordinates) const
    {
        return m_coordinatesToNode.find(coordinates) != m_coordinatesToNode.cend();
    }

    bool isDestination(const Coordinates& coordinates) const
    {
        return coordinates.first == m_width - 1 && coordinates.second == m_height - 1;
    }
};

RiskLevelRow parseRiskLevelLine(const std::string& line)
{
    RiskLevelRow riskLevelRow;
    std::transform(line.cbegin(), line.cend(), std::back_inserter(riskLevelRow), [](const char c)
                   {
                       return c - '0';
                   });

    return riskLevelRow;
}

RiskLevelMap parseRiskLevelLines(const std::vector<std::string>& riskLevelLines)
{
    RiskLevelMap riskLevelMap;

    for (const auto& line : riskLevelLines)
    {
        RiskLevelRow riskLevelRow = parseRiskLevelLine(line);
        riskLevelMap.push_back(std::move(riskLevelRow));
    }

    return riskLevelMap;
}

RiskLevelMap createRepeatedRiskLevelMap(const RiskLevelMap& originalRiskLevelMap, size_t scaleFactor)
{
    const size_t originalWidth = originalRiskLevelMap.at(0).size();
    const size_t originalHeight = originalRiskLevelMap.size();

    RiskLevelMap repeatedRiskLevelMap = RiskLevelMap(originalHeight * scaleFactor, RiskLevelRow(originalWidth * scaleFactor));

    for (size_t j = 0; j < originalHeight * scaleFactor; ++j)
    {
        for (size_t i = 0; i < originalWidth * scaleFactor; ++i)
        {
            const size_t xOriginal = i % originalWidth;
            const size_t yOriginal = j % originalHeight;
            const int baseRiskLevel = originalRiskLevelMap.at(yOriginal).at(xOriginal);
            const int additionalRiskLevel = (i / originalWidth) + (j / originalHeight);
            const int newRiskLevel = baseRiskLevel + additionalRiskLevel;
            const int newRiskLevelWrapped = ((newRiskLevel - 1) % 9) + 1;

            repeatedRiskLevelMap.at(j).at(i) = newRiskLevelWrapped;
        }
    }

    return repeatedRiskLevelMap;
}

int lowestTotalRiskOfAnyPath(const std::vector<std::string>& riskLevelLines)
{
    RiskLevelMap riskLevelMap = parseRiskLevelLines(riskLevelLines);
    CaveTraverser caveTraverser{std::move(riskLevelMap)};
    caveTraverser.traverse();
    return caveTraverser.getMinRiskLevel();
}

int lowestTotalRiskOfAnyPathRepeatedMap(const std::vector<std::string>& riskLevelLines)
{
    RiskLevelMap originalRiskLevelMap = parseRiskLevelLines(riskLevelLines);
    RiskLevelMap repeatedRiskLevelMap = createRepeatedRiskLevelMap(originalRiskLevelMap, SCALE_FACTOR_PART_TWO);
    CaveTraverser caveTraverser{std::move(repeatedRiskLevelMap)};
    caveTraverser.traverse();
    return caveTraverser.getMinRiskLevel();
}

}
}
}
