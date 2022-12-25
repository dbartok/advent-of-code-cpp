#include "Day24-BlizzardBasin.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <unordered_map>
#include <unordered_set>
#include <queue>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

unsigned FIRST_PART_NUM_SEGMENTS = 1;
unsigned SECOND_PART_NUM_SEGMENTS = 3;

}

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
    unsigned width;
    unsigned height;
    Coordinates startPosition;
    Coordinates endPosition;
    CoordinateToBlizzards xCoordinateToBlizzards;
    CoordinateToBlizzards yCoordinateToBlizzards;
};

class BlizzardPathfinder
{
public:
    BlizzardPathfinder(BlizzardMap blizzardMap, unsigned startTime)
        : m_blizzardMap{std::move(blizzardMap)}
        , m_startTime{startTime}
        , m_minTimeElapsedToReachGoal{std::numeric_limits<unsigned>::max()}
    {

    }

    void findShortestPath()
    {
        const auto cmp = [this](const Node& lhs, const Node& rhs)
        {
            return lhs.timeElapsed + lhs.remainingTimeLowerBound > rhs.timeElapsed + rhs.remainingTimeLowerBound;
        };

        const Node startNode{m_blizzardMap.startPosition, m_startTime, getRemainingTimeLowerBound(m_blizzardMap.startPosition)};

        std::priority_queue<Node, std::vector<Node>, decltype(cmp)> openNodesPriorityQueue(cmp);
        openNodesPriorityQueue.push(startNode);
        std::unordered_set<Node, NodeHash> seenNodes{startNode};

        while (!openNodesPriorityQueue.empty())
        {
            const Node currentNode = openNodesPriorityQueue.top();
            openNodesPriorityQueue.pop();

            if (currentNode.position == m_blizzardMap.endPosition)
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

    unsigned getMinTimeElapsedSinceStartToReachGoal() const
    {
        return m_minTimeElapsedToReachGoal - m_startTime;
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
    unsigned m_startTime;

    unsigned m_minTimeElapsedToReachGoal;

    unsigned getRemainingTimeLowerBound(const Coordinates& coordinates) const
    {
        return std::abs(m_blizzardMap.endPosition.first - coordinates.first) + std::abs(m_blizzardMap.endPosition.second - coordinates.second);
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
        if (position == m_blizzardMap.startPosition || position == m_blizzardMap.endPosition)
        {
            return true;
        }

        if (position.first < 1 || position.first >= m_blizzardMap.width - 1 || position.second < 1 || position.second >= m_blizzardMap.height - 1)
        {
            return false;
        }

        // Vertical blizzards
        if (!isBlizzardFree(m_blizzardMap.xCoordinateToBlizzards, position.first, position.second, time, m_blizzardMap.height))
        {
            return false;
        }

        // Horizontal blizzards
        if (!isBlizzardFree(m_blizzardMap.yCoordinateToBlizzards, position.second, position.first, time, m_blizzardMap.width))
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
    const unsigned width = blizzardMapLines.at(0).size();
    const unsigned height = blizzardMapLines.size();

    CoordinateToBlizzards xCoordinateToBlizzards;
    CoordinateToBlizzards yCoordinateToBlizzards;

    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
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

    Coordinates startPosition = GAP_OFFSET;
    Coordinates endPosition{width - 1 - GAP_OFFSET.first, height - 1 - GAP_OFFSET.second};

    return {width, height, std::move(startPosition), std::move(endPosition), std::move(xCoordinateToBlizzards), std::move(yCoordinateToBlizzards)};
}

unsigned fewestNumberOfMinutesForGivenNumberOfSegments(const std::vector<std::string>& blizzardMapLines, unsigned numSegments)
{
    BlizzardMap blizzardMap = parseBlizzardMapLines(blizzardMapLines);
    unsigned totalElapsedTime = 0;

    for (unsigned segment = 0; segment < numSegments; ++segment)
    {
        BlizzardPathfinder blizzardPathfinder{blizzardMap, totalElapsedTime};
        blizzardPathfinder.findShortestPath();
        totalElapsedTime += blizzardPathfinder.getMinTimeElapsedSinceStartToReachGoal();

        std::swap(blizzardMap.startPosition, blizzardMap.endPosition);
    }

    return totalElapsedTime;
}

unsigned fewestNumberOfMinutesToReachGoal(const std::vector<std::string>& blizzardMapLines)
{
    return fewestNumberOfMinutesForGivenNumberOfSegments(blizzardMapLines, 1);
}

unsigned fewestNumberOfMinutesToReachGoalAfterGoingBackForSnacks(const std::vector<std::string>& blizzardMapLines)
{
    return fewestNumberOfMinutesForGivenNumberOfSegments(blizzardMapLines, 3);
}

}
}
}