#include "Day10-PipeMaze.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>
#include <Eigen/dense>

#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const char VERTICAL_PIPE = '|';
const char HORIZONTAL_PIPE = '-';
const char NORTH_EAST_BEND = 'L';
const char NORTH_WEST_BEND = 'J';
const char SOUTH_WEST_BEND = '7';
const char SOUTH_EAST_BEND = 'F';
const char GROUND = '.';
const char STARTING_POSITION = 'S';

}

namespace AdventOfCode
{
namespace Year2023
{
namespace Day10
{

using Vector2D = Eigen::Vector2i;

struct Vector2DHash
{
    size_t operator()(const Vector2D& vec) const
    {
        std::size_t seed = 0;

        boost::hash_combine(seed, vec.x());
        boost::hash_combine(seed, vec.y());

        return seed;
    }
};

using Flow = std::pair<Vector2D, Vector2D>;

class PipeNetworkTraverser
{
public:
    PipeNetworkTraverser(std::vector<std::string> pipeNetwork)
        : m_pipeNetwork{std::move(pipeNetwork)}
        , m_height{m_pipeNetwork.size()}
        , m_width{m_pipeNetwork.at(0).size()}
    {
        initializePipeTypeToInwardsFlowMapping();

        for (int j = 0; j < m_height; ++j)
        {
            for (int i = 0; i < m_width; ++i)
            {
                if (tileAt({i, j}) == STARTING_POSITION)
                {
                    m_start = {i, j};
                }
            }
        }
    }

    int getMainLoopLength() const
    {
        const std::vector<Vector2D> validStartingDirections = getValidDirectionsFrom(m_start);

        Vector2D currentCoordinates = m_start;
        Vector2D currentDirection = validStartingDirections.front();
        int mainLoopLength = 1;

        while (currentCoordinates + currentDirection != m_start)
        {
            const Vector2D nextCoordinatesWithOneFlow = getStepResultIncludingOneFlow(currentCoordinates, currentDirection).get();

            currentCoordinates += currentDirection;
            currentDirection = nextCoordinatesWithOneFlow - currentCoordinates;

            ++mainLoopLength;
        }

        return mainLoopLength;
    }

private:
    std::vector<std::string> m_pipeNetwork;
    size_t m_height;
    size_t m_width;
    std::unordered_map<char, Flow> m_pipeTypeToInwardsFlow;
    Vector2D m_start;

    void initializePipeTypeToInwardsFlowMapping()
    {
        m_pipeTypeToInwardsFlow.emplace(VERTICAL_PIPE, std::make_pair(Vector2D{0, 1}, Vector2D{0, -1}));
        m_pipeTypeToInwardsFlow.emplace(HORIZONTAL_PIPE, std::make_pair(Vector2D{1, 0}, Vector2D{-1, 0}));
        m_pipeTypeToInwardsFlow.emplace(NORTH_EAST_BEND, std::make_pair(Vector2D{0, 1}, Vector2D{-1, 0}));
        m_pipeTypeToInwardsFlow.emplace(NORTH_WEST_BEND, std::make_pair(Vector2D{0, 1}, Vector2D{1, 0}));
        m_pipeTypeToInwardsFlow.emplace(SOUTH_EAST_BEND, std::make_pair(Vector2D{0, -1}, Vector2D{-1, 0}));
        m_pipeTypeToInwardsFlow.emplace(SOUTH_WEST_BEND, std::make_pair(Vector2D{0, -1}, Vector2D{1, 0}));
    }

    std::vector<Vector2D> getValidDirectionsFrom(const Vector2D& origin) const
    {
        std::vector<Vector2D> directionCandidates = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        std::vector<Vector2D> validDirections;
        for (const auto& directionCandidate : directionCandidates)
        {
            Vector2D neighborTileCoordinates = directionCandidate + origin;
            if (!isInbounds(neighborTileCoordinates))
            {
                continue;
            }

            boost::optional<Vector2D> stepResult = getStepResultIncludingOneFlow(origin, directionCandidate);
            if (stepResult)
            {
                validDirections.push_back(directionCandidate);
            }
        }
        return validDirections;
    }

    boost::optional<Vector2D> getStepResultIncludingOneFlow(const Vector2D& originTileCoordinates, const Vector2D& direction) const
    {
        const Vector2D neighborTileCoordinates = originTileCoordinates + direction;
        const char neighborTile = tileAt(neighborTileCoordinates);

        if (neighborTile == GROUND)
        {
            return boost::none;
        }

        const Flow& neighborFlow = m_pipeTypeToInwardsFlow.at(neighborTile);

        if (direction == neighborFlow.first)
        {
            return neighborTileCoordinates + (-1 * neighborFlow.second);
        }
        else if (direction == neighborFlow.second)
        {
            return neighborTileCoordinates + (-1 * neighborFlow.first);
        }
        else
        {
            return boost::none;
        }
    }

    char tileAt(const Vector2D& coordinates) const
    {
        return m_pipeNetwork.at(coordinates.y()).at(coordinates.x());
    }

    bool isInbounds(const Vector2D& coordinates) const
    {
        return coordinates.x() >= 0 && coordinates.y() >= 0 && coordinates.x() < m_width && coordinates.y() < m_height;
    }
};

int numStepsAlongLoopToFarthestPosition(const std::vector<std::string>& pipeNetworkLines)
{
    PipeNetworkTraverser pipeNetworkTraverser{pipeNetworkLines};

    return pipeNetworkTraverser.getMainLoopLength() / 2;
}

}
}
}