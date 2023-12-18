#include "Day17-ClumsyCrucible.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>

#include <algorithm>
#include <set>
#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2023
{
namespace Day17
{

using Coordinates = std::pair<int, int>;

Coordinates operator+(const Coordinates& lhs, const Coordinates& rhs)
{
    return {lhs.first + rhs.first, lhs.second + rhs.second};
}

const Coordinates LEFT = {-1, 0};
const Coordinates RIGHT = {1, 0};
const Coordinates UP = {0, -1};
const Coordinates DOWN = {0, 1};

struct Node
{
    Coordinates position;
    Coordinates arrivalDirection;
    int numTilesMovedInDirection;

    mutable int pathCost;

    bool operator==(const Node& other) const
    {
        return this->position == other.position
            && this->arrivalDirection == other.arrivalDirection
            && this->numTilesMovedInDirection == other.numTilesMovedInDirection;
    }
};

struct NodeHash
{
    size_t operator()(const Node& node) const
    {
        std::size_t seed = 0;

        boost::hash_combine(seed, node.position);
        boost::hash_combine(seed, node.arrivalDirection);
        boost::hash_combine(seed, node.numTilesMovedInDirection);

        return seed;
    }
};

using NodeUnorderedSet = std::unordered_set<Node, NodeHash>;

struct NodePtrCostCmp
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        if (lhs->pathCost < rhs->pathCost)
        {
            return true;
        }

        if (lhs->pathCost > rhs->pathCost)
        {
            return false;
        }

        return lhs < rhs;
    }
};


class CityBlockTraverser
{
public:
    CityBlockTraverser(std::vector<std::vector<int>> map, int minNumMovesInSingleDirection, int maxNumMovesInSingleDirection)
        : m_map{std::move(map)}
        , m_width{m_map.front().size()}
        , m_height{m_map.size()}
        , m_minNumMovesInSingleDirection{minNumMovesInSingleDirection}
        , m_maxNumMovesInSingleDirection{maxNumMovesInSingleDirection}
    {

    }

    void traverse()
    {
        Node startNode{{0, 0}, RIGHT, 0, 0};
        auto startInsertionIter = m_allNodes.insert(startNode).first;
        const Node& insertedStartNode = *startInsertionIter;
        m_activeNodes.insert(&insertedStartNode);

        while (true)
        {
            auto closestNodePtrIter = m_activeNodes.cbegin();
            const auto closestNodePtr = *closestNodePtrIter;
            m_activeNodes.erase(closestNodePtrIter);

            if (closestNodePtr->position == Coordinates{m_width - 1, m_height - 1} && closestNodePtr->numTilesMovedInDirection >= m_minNumMovesInSingleDirection)
            {
                m_minHeatLoss = closestNodePtr->pathCost;
                return;
            }

            for (const auto& neighborNode : getAllNeighborNodes(*closestNodePtr))
            {
                auto existingNeighborNodeIter = m_allNodes.find(neighborNode);

                if (existingNeighborNodeIter != m_allNodes.cend())
                {
                    if (neighborNode.pathCost < existingNeighborNodeIter->pathCost)
                    {
                        const Node& existingNeighborNode = *existingNeighborNodeIter;
                        m_activeNodes.erase(&existingNeighborNode);
                        existingNeighborNodeIter->pathCost = neighborNode.pathCost;
                        m_activeNodes.insert(&existingNeighborNode);
                    }
                }
                else
                {
                    auto neighborInsertionIter = m_allNodes.insert(neighborNode).first;
                    const Node& insertedNeighborNode = *neighborInsertionIter;
                    m_activeNodes.insert(&insertedNeighborNode);
                }
            }
        }
    }

    int getMinHeatLoss() const
    {
        return m_minHeatLoss;
    }

private:
    std::vector<std::vector<int>> m_map;
    size_t m_width;
    size_t m_height;
    int m_minNumMovesInSingleDirection;
    int m_maxNumMovesInSingleDirection;

    int m_minHeatLoss = std::numeric_limits<int>::max();

    NodeUnorderedSet m_allNodes;
    std::set<const Node*, NodePtrCostCmp> m_activeNodes;

    std::vector<Node> getAllNeighborNodes(const Node& baseNode) const
    {
        std::vector<Node> allNeighborNodes;

        if (baseNode.numTilesMovedInDirection < m_maxNumMovesInSingleDirection)
        {
            auto maybeForwardsNode = getNodeInDirection(baseNode, baseNode.arrivalDirection, baseNode.numTilesMovedInDirection + 1);
            if (maybeForwardsNode)
            {
                allNeighborNodes.push_back(maybeForwardsNode.get());
            }
        }

        if (baseNode.numTilesMovedInDirection == 0 || baseNode.numTilesMovedInDirection >= m_minNumMovesInSingleDirection)
        {
            const Coordinates firstTurnDirection = {baseNode.arrivalDirection.second, baseNode.arrivalDirection.first};
            auto maybeFirstTurnNode = getNodeInDirection(baseNode, firstTurnDirection, 1);
            if (maybeFirstTurnNode)
            {
                allNeighborNodes.push_back(maybeFirstTurnNode.get());
            }


            const Coordinates secondTurnDirection = {-baseNode.arrivalDirection.second, -baseNode.arrivalDirection.first};
            auto maybeSecondTurnNode = getNodeInDirection(baseNode, secondTurnDirection, 1);
            if (maybeSecondTurnNode)
            {
                allNeighborNodes.push_back(maybeSecondTurnNode.get());
            }
        }

        return allNeighborNodes;
    }

    boost::optional<Node> getNodeInDirection(const Node& baseNode, const Coordinates& direction, int newNumTilesMovedInDirection) const
    {
        const Coordinates newPosition = baseNode.position + direction;

        if (newPosition.first < 0 || newPosition.second < 0 || newPosition.first >= m_width || newPosition.second >= m_height)
        {
            return boost::none;
        }

        return Node{newPosition, direction, newNumTilesMovedInDirection, baseNode.pathCost + getCostAt(newPosition)};
    }

    int getCostAt(const Coordinates& coordinates) const
    {
        return m_map.at(coordinates.second).at(coordinates.first);
    }
};

std::vector<std::vector<int>> parseMapLines(const std::vector<std::string>& mapLines)
{
    std::vector<std::vector<int>> map(mapLines.size(), std::vector<int>(mapLines.front().size()));

    for (int j = 0; j < mapLines.size(); ++j)
    {
        for (int i = 0; i < mapLines.front().size(); ++i)
        {
            map.at(j).at(i) = mapLines.at(j).at(i) - '0';
        }
    }

    return map;
}

int leastHeatLossIncurred(const std::vector<std::string>& mapLines)
{
    std::vector<std::vector<int>> map = parseMapLines(mapLines);

    CityBlockTraverser cityBlockTraverser{std::move(map), 1, 3};

    cityBlockTraverser.traverse();

    return cityBlockTraverser.getMinHeatLoss();
}

int leastHeatLossIncurredForUltraCrucible(const std::vector<std::string>& mapLines)
{
    std::vector<std::vector<int>> map = parseMapLines(mapLines);

    CityBlockTraverser cityBlockTraverser{std::move(map), 4, 10};

    cityBlockTraverser.traverse();

    return cityBlockTraverser.getMinHeatLoss();
}

}
}
}