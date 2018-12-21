#include "Day20-ARegularMap.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

struct RoomNode;
using Coordinates = std::pair<int, int>;
using CoordinatesSet = std::unordered_set<Coordinates, boost::hash<Coordinates>>;
using CoordinatesToNeighborCoordinates = std::unordered_map<Coordinates, CoordinatesSet, boost::hash<Coordinates>>;

class RoomLayoutRegexParser
{
public:
    RoomLayoutRegexParser(std::string roomLayoutRegex)
        : m_roomLayoutRegex{std::move(roomLayoutRegex)}
        , m_layoutMap{}
    {

    }

    void parse()
    {
        Coordinates currentlyParsedCoordinates{0, 0};
        m_layoutMap.emplace(currentlyParsedCoordinates, CoordinatesSet{});
        std::stack<Coordinates> backtrackStack;

        for (size_t i = 1; i < m_roomLayoutRegex.length() - 1; ++i)
        {
            char c = m_roomLayoutRegex[i];
            if (c == '(')
            {
                backtrackStack.push(currentlyParsedCoordinates);
            }
            else if (c == ')')
            {
                currentlyParsedCoordinates = backtrackStack.top();
                backtrackStack.pop();
            }
            else if (c == '|')
            {
                currentlyParsedCoordinates = backtrackStack.top();
            }
            else
            {
                Coordinates neighborCoordinates = getNeighborCoordinates(currentlyParsedCoordinates, c);
                m_layoutMap[currentlyParsedCoordinates].insert(std::move(neighborCoordinates));

                currentlyParsedCoordinates = neighborCoordinates;
            }
        }
    }

    const CoordinatesToNeighborCoordinates& getLayoutMap() const
    {
        return m_layoutMap;
    }

private:
    std::string m_roomLayoutRegex;
    CoordinatesToNeighborCoordinates m_layoutMap;

    Coordinates getNeighborCoordinates(const Coordinates& coordinates, char neighborDirection)
    {
        Coordinates neighborCoordinates{coordinates};

        switch (neighborDirection)
        {
            case 'N':
                neighborCoordinates.first -= 1;
                break;

            case 'S':
                neighborCoordinates.first += 1;
                break;

            case 'W':
                neighborCoordinates.second -= 1;
                break;

            case 'E':
                neighborCoordinates.second += 1;
                break;
        }

        return neighborCoordinates;
    }
};

class CoordinatesTraverser
{
public:
    CoordinatesTraverser(CoordinatesToNeighborCoordinates layoutMap)
        : m_layoutMap{std::move(layoutMap)}
    {

    }

    void traverse()
    {
        std::queue<Coordinates> reachableNodesQueue;
        std::queue<Coordinates> nextReachableNodesQueue;
        reachableNodesQueue.push(Coordinates{0, 0});
        unsigned numNodesInLongestPath = 0;

        while (!reachableNodesQueue.empty())
        {
            ++numNodesInLongestPath;
            while (!reachableNodesQueue.empty())
            {
                Coordinates currentCoordinates = reachableNodesQueue.front();
                reachableNodesQueue.pop();
                CoordinatesSet neighbors;

                const auto findResult = m_layoutMap.find(currentCoordinates);
                if (findResult != m_layoutMap.end())
                {
                    neighbors = findResult->second;
                }

                for (const auto& neighbor : neighbors)
                {
                    bool notVisited = m_visited.insert(neighbor).second;
                    if (notVisited)
                    {
                        nextReachableNodesQueue.push(neighbor);
                    }
                }
            }

            std::swap(reachableNodesQueue, nextReachableNodesQueue);
        }

        m_maxDistance = numNodesInLongestPath - 1;
    }

    unsigned getDistanceToFarthestCoordinate() const
    {
        return m_maxDistance;
    }

private:
    CoordinatesToNeighborCoordinates m_layoutMap;
    CoordinatesSet m_visited;

    unsigned m_maxDistance = 0;
};

unsigned distanceToFarthestRoom(const std::string& roomLayoutRegex)
{
    RoomLayoutRegexParser roomLayoutRegexParser{roomLayoutRegex};
    roomLayoutRegexParser.parse();

    auto layoutMap = roomLayoutRegexParser.getLayoutMap();

    CoordinatesTraverser coordinatesTraverser{std::move(layoutMap)};
    coordinatesTraverser.traverse();
    return coordinatesTraverser.getDistanceToFarthestCoordinate();
}

}
