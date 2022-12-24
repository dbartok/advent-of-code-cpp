#include "Day24-BlizzardBasin.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <unordered_map>
#include <unordered_set>
#include <queue>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2022
{
namespace Day24
{

struct Blizzard
{
    int initialOffset;
    int velocity;

    Blizzard(int initialOffset, int velocity)
        : initialOffset{initialOffset}
        , velocity{velocity}
    {

    }
};

using Blizzards = std::vector<Blizzard>;
using CoordinateToBlizzards = std::unordered_map<int, Blizzards>;
using Coordinates = std::pair<int, int>;

Coordinates GAP_OFFSET{1, 0};

struct BlizzardMap
{
    CoordinateToBlizzards xCoordinateToBlizzards;
    CoordinateToBlizzards yCoordinateToBlizzards;
};

class BlizzardPathfinder
{
public:
    BlizzardPathfinder(BlizzardMap blizzardMap, unsigned width, unsigned height)
        : m_blizzardMap{std::move(blizzardMap)}
        , m_width{width}
        , m_height{height}
        , m_startPosition{GAP_OFFSET}
        , m_endPosition{width - 1 - GAP_OFFSET.first, height - 1 - GAP_OFFSET.second}
        , m_minTimeElapsedToReachGoal{std::numeric_limits<unsigned>::max()}
    {

    }

    void findShortestPath()
    {
        const auto cmp = [this](const Node& lhs, const Node& rhs)
        {
            return lhs.timeElapsed + lhs.remainingTimeLowerBound > rhs.timeElapsed + rhs.remainingTimeLowerBound;
        };

        const Node startNode{m_startPosition, 0, getRemainingTimeLowerBound(m_startPosition)};

        std::priority_queue<Node, std::vector<Node>, decltype(cmp)> openNodesPriorityQueue(cmp);
        openNodesPriorityQueue.push(startNode);
        std::unordered_set<Node, NodeHash> seenNodes{startNode};

        while (!openNodesPriorityQueue.empty())
        {
            const Node currentNode = openNodesPriorityQueue.top();
            openNodesPriorityQueue.pop();

            if (currentNode.position == m_endPosition)
            {
                m_minTimeElapsedToReachGoal = currentNode.timeElapsed;
                break;
            }

            const std::vector<Node> neighborNodes = getNeighborNodes(currentNode);

            for (const auto& neighborNode : neighborNodes)
            {
                if (seenNodes.insert(neighborNode).second)
                {
                    openNodesPriorityQueue.push(neighborNode);
                }
            }
        }
    }

    unsigned getMinTimeElapsedToReachGoal() const
    {
        return m_minTimeElapsedToReachGoal;
    }

private:
    struct Node
    {
        Coordinates position;
        unsigned timeElapsed;
        unsigned remainingTimeLowerBound;

        bool operator==(const Node& other) const
        {
            return position == other.position && timeElapsed == other.timeElapsed;
        }

        Node(Coordinates position, unsigned timeElapsed, unsigned remainingTimeLowerBound)
            : position{std::move(position)}
            , timeElapsed{timeElapsed}
            , remainingTimeLowerBound{remainingTimeLowerBound}
        {

        }
    };

    struct NodeHash
    {
        size_t operator()(const Node& node) const
        {
            std::size_t seed = 0;

            boost::hash_combine(seed, node.position);
            boost::hash_combine(seed, node.timeElapsed);

            return seed;
        }
    };

    BlizzardMap m_blizzardMap;

    unsigned m_minTimeElapsedToReachGoal;
    unsigned m_width;
    unsigned m_height;
    Coordinates m_startPosition;
    Coordinates m_endPosition;

    unsigned getRemainingTimeLowerBound(const Coordinates& coordinates) const
    {
        return std::abs(m_endPosition.first - coordinates.first) + std::abs(m_endPosition.second - coordinates.second);
    }

    std::vector<Node> getNeighborNodes(const Node& currentNode) const
    {
        const auto& currentPosition = currentNode.position;
        std::vector<Coordinates> possibleNextPositions =
        {
            {currentPosition.first + 1, currentPosition.second},
            {currentPosition.first - 1, currentPosition.second},
            {currentPosition.first, currentPosition.second + 1},
            {currentPosition.first, currentPosition.second - 1},
            {currentPosition.first, currentPosition.second}, // Wait in place
        };
        unsigned timeElapsedAfterMove = currentNode.timeElapsed + 1;

        std::vector<Node> neighborNodes;

        for (const auto& possibleNextPosition : possibleNextPositions)
        {
            if (isPositionFreeAtTime(possibleNextPosition, timeElapsedAfterMove))
            {
                neighborNodes.emplace_back(possibleNextPosition, timeElapsedAfterMove, getRemainingTimeLowerBound(possibleNextPosition));
            }
        }

        return neighborNodes;
    }

    bool isPositionFreeAtTime(const Coordinates& position, unsigned time) const
    {
        if (position == m_startPosition || position == m_endPosition)
        {
            return true;
        }

        if (position.first < 1 || position.first >= m_width - 1 || position.second < 1 || position.second >= m_height - 1)
        {
            return false;
        }

        // Vertical blizzards
        if (!isBlizzardFree(m_blizzardMap.xCoordinateToBlizzards, position.first, position.second, time, m_height))
        {
            return false;
        }

        // Horizontal blizzards
        if (!isBlizzardFree(m_blizzardMap.yCoordinateToBlizzards, position.second, position.first, time, m_width))
        {
            return false;
        }

        return true;
    }

    static bool isBlizzardFree(const CoordinateToBlizzards& coordinateToBlizzards, int blizzardMapKey, int positionToCheck, unsigned time, unsigned arrayLength)
    {
        auto blizzardsIter = coordinateToBlizzards.find(blizzardMapKey);

        if (blizzardsIter == coordinateToBlizzards.end())
        {
            return true;
        }

        const auto& blizzards = blizzardsIter->second;

        for (const auto& blizzard : blizzards)
        {
            const int blizzardAbsolutePositionWithoutWrapping = blizzard.initialOffset + blizzard.velocity * time;
            const int blizzardPositionRelativeToBoundWithoutWrapping = blizzardAbsolutePositionWithoutWrapping - 1;
            const int wrappingLength = arrayLength - 2;
            const int wrappedBlizzardCoordinateRelativeToBound = mod(blizzardPositionRelativeToBoundWithoutWrapping, wrappingLength);
            const int currentBlizzardCoordinate = wrappedBlizzardCoordinateRelativeToBound + 1;

            if (positionToCheck == currentBlizzardCoordinate)
            {
                return false;
            }
        }

        return true;
    }

    static int mod(int a, int b)
    {
        int m = a % b;

        if (m < 0)
        {
            m += b;
        }

        return m;
    }
};

BlizzardMap parseBlizzardMapLines(const std::vector<std::string>& blizzardMapLines)
{
    CoordinateToBlizzards xCoordinateToBlizzards;
    CoordinateToBlizzards yCoordinateToBlizzards;

    for (int j = 0; j < blizzardMapLines.size(); ++j)
    {
        for (int i = 0; i < blizzardMapLines.at(j).size(); ++i)
        {
            switch (blizzardMapLines.at(j).at(i))
            {
                case '>':
                    yCoordinateToBlizzards[j].emplace_back(i, 1);
                    break;
                case '<':
                    yCoordinateToBlizzards[j].emplace_back(i, -1);
                    break;
                case 'v':
                    xCoordinateToBlizzards[i].emplace_back(j, 1);
                    break;
                case '^':
                    xCoordinateToBlizzards[i].emplace_back(j, -1);
                    break;
                default:
                    break;
            }
        }
    }

    return {xCoordinateToBlizzards, yCoordinateToBlizzards};
}

unsigned fewestNumberOfMinutesToReachGoal(const std::vector<std::string>& blizzardMapLines)
{
    BlizzardMap blizzardMap = parseBlizzardMapLines(blizzardMapLines);

    BlizzardPathfinder blizzardPathfinder{std::move(blizzardMap), blizzardMapLines.at(0).size(), blizzardMapLines.size()};

    blizzardPathfinder.findShortestPath();

    return blizzardPathfinder.getMinTimeElapsedToReachGoal();
}

}
}
}