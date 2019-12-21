#include "Day20-DonutMaze.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash/hash.hpp>

#include <stdexcept>
#include <queue>
#include <unordered_map>
#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using MazeMap = std::vector<std::string>;
using Coordinates = std::pair<int, int>;

struct DonutMaze
{
    MazeMap map;
    Coordinates start;
    Coordinates end;
    std::unordered_map<Coordinates, Coordinates, boost::hash<Coordinates>> portalToDestination;
};

using PortalNameToEndpoints = std::unordered_map<std::string, std::vector<Coordinates>>;

class DonutMazeParser
{
public:
    DonutMazeParser(std::vector<std::string> donutMazeLines)
        : m_map{std::move(donutMazeLines)}
    {

    }

    void parse()
    {
        for (int j = 0; j < m_map.size(); ++j)
        {
            for (int i = 0; i < m_map.at(j).size(); ++i)
            {
                if (isPortalElement({i, j}))
                {
                    processPortal({i, j});
                }
            }
        }
    }

    DonutMaze getDonutMaze()
    {
        DonutMaze donutMaze;
        donutMaze.map = m_map;

        for (const auto& portalNameAndEndpoints : m_portalNameToEndpoints)
        {
            std::string portalName = portalNameAndEndpoints.first;
            std::vector<Coordinates> endpoints = portalNameAndEndpoints.second;
            if (portalName == "AA")
            {
                donutMaze.start = endpoints.front();
            }
            else if (portalName == "ZZ")
            {
                donutMaze.end = endpoints.front();
            }
            else
            {
                Coordinates source = endpoints.at(0);
                Coordinates destination = endpoints.at(1);
                donutMaze.portalToDestination[source] = destination;
                donutMaze.portalToDestination[destination] = source;
            }
        }

        return donutMaze;
    }

private:
    MazeMap m_map;
    PortalNameToEndpoints m_portalNameToEndpoints;

    void processPortal(const Coordinates& firstLetterCoordinates)
    {
        Coordinates secondLetterCoordinates = getSecondLetterCoordinates(firstLetterCoordinates);
        Coordinates connectedTileCoordinates = getConnectedTileCoordinates({firstLetterCoordinates, secondLetterCoordinates});

        std::string portalName = {atCoordinates(firstLetterCoordinates), atCoordinates(secondLetterCoordinates)};

        m_portalNameToEndpoints[portalName].push_back(connectedTileCoordinates);

        m_map.at(firstLetterCoordinates.second).at(firstLetterCoordinates.first) = ' ';
        m_map.at(secondLetterCoordinates.second).at(secondLetterCoordinates.first) = ' ';
    }

    Coordinates getSecondLetterCoordinates(const Coordinates& firstLetterCoordinates) const
    {
        Coordinates rightNeighbor{firstLetterCoordinates.first + 1, firstLetterCoordinates.second};
        if (isPortalElement(rightNeighbor))
        {
            return rightNeighbor;
        }

        Coordinates downNeighbor{firstLetterCoordinates.first, firstLetterCoordinates.second + 1};
        return downNeighbor;
    }

    Coordinates getConnectedTileCoordinates(const std::vector<Coordinates>& portalCoordinates)
    {
        std::vector<Coordinates> candidateTileCoordinates;
        for (const auto& coord : portalCoordinates)
        {
            std::vector<Coordinates> neighborCoordinates = getNeighborCoordinates(coord);
            std::copy(neighborCoordinates.begin(), neighborCoordinates.end(), std::back_inserter(candidateTileCoordinates));
        }

        for (const auto& coords : candidateTileCoordinates)
        {
            if (isFreeTile(coords))
            {
                return coords;
            }
        }

        throw std::runtime_error("Cannot find connected tile");
    }

    std::vector<Coordinates> getNeighborCoordinates(const Coordinates& coordinates)
    {
        std::vector<Coordinates> neighbors;
        neighbors.push_back({coordinates.first + 1, coordinates.second});
        neighbors.push_back({coordinates.first - 1, coordinates.second});
        neighbors.push_back({coordinates.first, coordinates.second + 1});
        neighbors.push_back({coordinates.first, coordinates.second - 1});
        return neighbors;
    }

    bool isPortalElement(const Coordinates& coordinates) const
    {
        return std::isalpha(atCoordinates(coordinates));
    }

    bool isFreeTile(const Coordinates& coordinates) const
    {
        return atCoordinates(coordinates) == '.';
    }

    char atCoordinates(const Coordinates& coordinates) const
    {
        if (coordinates.second < 0 || coordinates.second >= m_map.size() || coordinates.first < 0 || coordinates.first >= m_map.at(coordinates.second).size())
        {
            return '#';
        }

        return m_map.at(coordinates.second).at(coordinates.first);
    }
};

struct DonutMazeSearchNode
{
    Coordinates coordinates;
    int layer;
    int numSteps;

    bool operator==(const DonutMazeSearchNode& other) const
    {
        return coordinates == other.coordinates && layer == other.layer;
    }
};

struct DonutMazeSearchNodeHash
{
    std::size_t operator()(const DonutMazeSearchNode& node) const
    {
        std::size_t seed = 0;

        boost::hash_combine(seed, node.coordinates);
        boost::hash_combine(seed, node.layer);

        return seed;
    }
};

enum class TraverseMode
{
    IGNORE_LAYERS,
    RESPECT_LAYERS,
};

class DonutMazeTraverser
{
public:
    DonutMazeTraverser(DonutMaze donutMaze, TraverseMode traverseMode = TraverseMode::IGNORE_LAYERS)
        : m_maze{std::move(donutMaze)}
        , m_traverseMode{traverseMode}
    {

    }

    void traverse()
    {
        std::queue<DonutMazeSearchNode> bfsQueue;
        DonutMazeSearchNode initialNode{m_maze.start, 0};
        bfsQueue.push(initialNode);

        std::unordered_set<DonutMazeSearchNode, DonutMazeSearchNodeHash> visited;
        visited.insert(initialNode);

        while (!bfsQueue.empty())
        {
            DonutMazeSearchNode currentNode = bfsQueue.front();
            bfsQueue.pop();

            if (hasReachedGoal(currentNode))
            {
                m_shortestPathLength = currentNode.numSteps;
                return;
            }

            std::vector<DonutMazeSearchNode> neighborNodes = createNeighborNodes(currentNode);

            for (auto& node : neighborNodes)
            {
                if (visited.find(node) == visited.cend())
                {
                    visited.insert(node);
                    bfsQueue.push(node);
                }
            }
        }

        throw std::runtime_error("Unable to find exit.");
    }

    int getShortestPathLength() const
    {
        return m_shortestPathLength;
    }

private:
    DonutMaze m_maze;
    TraverseMode m_traverseMode;

    int m_shortestPathLength = std::numeric_limits<int>::max();

    std::vector<DonutMazeSearchNode> createNeighborNodes(const DonutMazeSearchNode& currentNode) const
    {
        std::vector<DonutMazeSearchNode> neighborNodes;

        const auto portalFindResult = m_maze.portalToDestination.find(currentNode.coordinates);
        if (portalFindResult != m_maze.portalToDestination.cend())
        {
            int layerModifier = getLayerModifier(currentNode.coordinates);
            int newLayer = currentNode.layer + layerModifier;
            if (m_traverseMode == TraverseMode::IGNORE_LAYERS || newLayer >= 0)
            {
                neighborNodes.push_back({DonutMazeSearchNode{portalFindResult->second, newLayer, currentNode.numSteps + 1}});
            }
        }

        std::vector<Coordinates> neighborCoordinates = getNeighborCoordinates(currentNode.coordinates);
        for (auto& coord : neighborCoordinates)
        {
            if (m_maze.map.at(coord.second).at(coord.first) == '.')
            {
                neighborNodes.push_back(DonutMazeSearchNode{std::move(coord), currentNode.layer, currentNode.numSteps + 1});
            }
        }

        return neighborNodes;
    }

    std::vector<Coordinates> getNeighborCoordinates(const Coordinates& currentCoordinates) const
    {
        std::vector<Coordinates> neighborCoordinates;
        neighborCoordinates.push_back({currentCoordinates.first + 1, currentCoordinates.second});
        neighborCoordinates.push_back({currentCoordinates.first - 1, currentCoordinates.second});
        neighborCoordinates.push_back({currentCoordinates.first, currentCoordinates.second + 1});
        neighborCoordinates.push_back({currentCoordinates.first, currentCoordinates.second - 1});
        return neighborCoordinates;
    }

    bool hasReachedGoal(const DonutMazeSearchNode& node) const
    {
        if (node.coordinates == m_maze.end)
        {
            if (m_traverseMode == TraverseMode::RESPECT_LAYERS && node.layer != 0)
            {
                return false;
            }

            return true;
        }

        return false;
    }

    int getLayerModifier(const Coordinates& coordinates) const
    {
        bool isOnOuterRing = coordinates.first == 2 ||
            coordinates.first == m_maze.map.at(coordinates.second).size() - 3 ||
            coordinates.second == 2 ||
            coordinates.second == m_maze.map.size() - 3;

        if (isOnOuterRing)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
};

DonutMaze createDonutMaze(const std::vector<std::string>& donutMazeLines)
{
    DonutMazeParser parser{donutMazeLines};
    parser.parse();
    return parser.getDonutMaze();
}

int numStepsToExit(const std::vector<std::string>& donutMazeLines)
{
    DonutMazeTraverser traverser{createDonutMaze(donutMazeLines)};

    traverser.traverse();

    return traverser.getShortestPathLength();
}

int numStepsToExitWithMatchingLayers(const std::vector<std::string>& donutMazeLines)
{
    DonutMazeTraverser traverser{createDonutMaze(donutMazeLines), TraverseMode::RESPECT_LAYERS};

    traverser.traverse();

    return traverser.getShortestPathLength();
}

}
