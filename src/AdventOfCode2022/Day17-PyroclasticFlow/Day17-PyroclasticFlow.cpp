#include "Day17-PyroclasticFlow.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <vector>
#include <stdexcept>
#include <unordered_set>
#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

int CHAMBER_WIDTH = 7;
int ROCK_SPAWN_OFFSET_FROM_LEFT_OF_CHAMBER = 2;
int ROCK_SPAWN_OFFSET_FROM_TOP_OF_ROCKS = 4;
unsigned NUM_ROCKS_TO_SIMULATE = 2022;
unsigned NUM_ROCK_TYPES = 5;

}

namespace AdventOfCode
{
namespace Year2022
{
namespace Day17
{

enum class Direction
{
    LEFT,
    RIGHT,
};

enum class RockType
{
    HYPHEN = 0,
    PLUS = 1,
    MIRRORED_L = 2,
    PILLAR = 3,
    SQUARE = 4,
};

using Coordinates = std::pair<int, int>;
using CoordinatesSet = std::unordered_set<Coordinates, boost::hash<Coordinates>>;

class Rock
{
public:
    Rock(RockType type, const Coordinates& bottomLeftCornerPosition)
    {
        switch (type)
        {
            case RockType::HYPHEN:
                inializeHyphenRock(bottomLeftCornerPosition);
                break;
            case RockType::PLUS:
                initializePlusRock(bottomLeftCornerPosition);
                break;
            case RockType::MIRRORED_L:
                initializeMirroredLRock(bottomLeftCornerPosition);
                break;
            case RockType::PILLAR:
                initializePillarRock(bottomLeftCornerPosition);
                break;
            case RockType::SQUARE:
                initializeSquareRock(bottomLeftCornerPosition);
                break;
            default:
                throw std::runtime_error("Invalid rock  type: " + std::to_string(static_cast<int>(type)));
        }
    }

    bool applyJet(Direction direction, const CoordinatesSet& occupiedCoordinates)
    {
        if (direction == Direction::LEFT)
        {
            return applyOffset({-1, 0}, occupiedCoordinates);
        }
        else if (direction == Direction::RIGHT)
        {
            return applyOffset({1, 0}, occupiedCoordinates);
        }
    }

    bool applyFallUnit(const CoordinatesSet& occupiedCoordinates)
    {
        return applyOffset({0, -1}, occupiedCoordinates);
    }

    const std::vector<Coordinates>& getAllCoordinates() const
    {
        return m_allCoordinates;
    }

private:
    std::vector<Coordinates> m_allCoordinates;

    void inializeHyphenRock(const Coordinates& bottomLeftCornerPosition)
    {
        const std::vector<Coordinates> offsets = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};
        initializeRock(bottomLeftCornerPosition, offsets);
    }

    void initializePlusRock(const Coordinates& bottomLeftCornerPosition)
    {
        const std::vector<Coordinates> offsets = {{0, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 1}};
        initializeRock(bottomLeftCornerPosition, offsets);
    }

    void initializeMirroredLRock(const Coordinates& bottomLeftCornerPosition)
    {
        const std::vector<Coordinates> offsets = {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}};
        initializeRock(bottomLeftCornerPosition, offsets);
    }

    void initializePillarRock(const Coordinates& bottomLeftCornerPosition)
    {
        const std::vector<Coordinates> offsets = {{0, 0}, {0, 1}, {0, 2}, {0, 3}};
        initializeRock(bottomLeftCornerPosition, offsets);
    }

    void initializeSquareRock(const Coordinates& bottomLeftCornerPosition)
    {
        const std::vector<Coordinates> offsets = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
        initializeRock(bottomLeftCornerPosition, offsets);
    }

    void initializeRock(const Coordinates& bottomLeftCornerPosition, const std::vector<Coordinates>& allRelativeStartingCoordinates)
    {
        m_allCoordinates = allRelativeStartingCoordinates;
        applyOffset(bottomLeftCornerPosition, {});
    }

    bool applyOffset(const Coordinates& offset, const CoordinatesSet& occupiedCoordinates)
    {
        std::vector<Coordinates> allCoordinatesAfterMove;

        for (auto& coordinates : m_allCoordinates)
        {
            allCoordinatesAfterMove.emplace_back(coordinates.first + offset.first, coordinates.second + offset.second);
        }

        if (isValidMove(allCoordinatesAfterMove, occupiedCoordinates))
        {
            m_allCoordinates = allCoordinatesAfterMove;
            return true;
        }
        else
        {
            return false;
        }
    }

    static bool isValidMove(std::vector<Coordinates>& allCoordinatesAfterMove, const CoordinatesSet& occupiedCoordinates)
    {
        return std::all_of(allCoordinatesAfterMove.cbegin(), allCoordinatesAfterMove.cend(), [occupiedCoordinates](const auto& coords)
                           {
                               const bool isInBounds = coords.first >= 0 && coords.first < CHAMBER_WIDTH  && coords.second >= 0;
                               const bool isUnoccupied = occupiedCoordinates.find(coords) == occupiedCoordinates.cend();

                               return isInBounds && isUnoccupied;
                           });
    }
};

class FallingRockSimulator
{
public:
    FallingRockSimulator(std::vector<Direction> jetPattern)
        : m_jetPattern{std::move(jetPattern)}
    {

    }

    void simulate()
    {
        for (size_t numRocksSettled = 0; numRocksSettled < NUM_ROCKS_TO_SIMULATE; ++numRocksSettled)
        {
            const RockType nextRockType = static_cast<RockType>(numRocksSettled % NUM_ROCK_TYPES);
            const int spawnY = getMaxYCoordinate() + ROCK_SPAWN_OFFSET_FROM_TOP_OF_ROCKS;
            Rock rock{nextRockType, {ROCK_SPAWN_OFFSET_FROM_LEFT_OF_CHAMBER, spawnY}};

            settleRock(std::move(rock));
        }
    }

    int getTowerHeight() const
    {
        return getMaxYCoordinate() + 1;
    }

private:
    CoordinatesSet m_occupiedPositions;
    size_t m_numPushesSimulated = 0;

    std::vector<Direction> m_jetPattern;

    void settleRock(Rock rock)
    {
        do
        {
            const Direction nextJetDirection = m_jetPattern.at(m_numPushesSimulated++ % m_jetPattern.size());
            rock.applyJet(nextJetDirection, m_occupiedPositions);
        } while (rock.applyFallUnit(m_occupiedPositions));

        const auto& rockCoordinates = rock.getAllCoordinates();

        m_occupiedPositions.insert(rockCoordinates.cbegin(), rockCoordinates.cend());
    }

    bool isColliding(const Rock& rock) const
    {
        const auto& rockCoordinates = rock.getAllCoordinates();

        return std::any_of(rockCoordinates.cbegin(), rockCoordinates.cend(), [this](const auto& coords)
                           {
                               return coords.second < 0 || this->m_occupiedPositions.find(coords) != this->m_occupiedPositions.cend();
                           });
    }

    int getMaxYCoordinate() const
    {
        const auto maxYIter = std::max_element(m_occupiedPositions.cbegin(), m_occupiedPositions.cend(), [](const auto& lhs, const auto& rhs) {return lhs.second < rhs.second; });
        return maxYIter != m_occupiedPositions.cend() ? maxYIter->second : -1;
    }
};

std::vector<Direction> parseJetPatternString(const std::string& jetPatternString)
{
    std::vector<Direction> jetPattern;

    for (const char c : jetPatternString)
    {
        if (c == '<')
        {
            jetPattern.push_back(Direction::LEFT);
        }
        else if (c == '>')
        {
            jetPattern.push_back(Direction::RIGHT);
        }
        else
        {
            throw std::runtime_error("Invalid character: " + std::string{c});
        }
    }

    return jetPattern;
}

int towerHeightAfterRocksStoppedFalling(const std::string& jetPatternString)
{
    std::vector<Direction> jetPattern = parseJetPatternString(jetPatternString);

    FallingRockSimulator fallingRockSimulator{std::move(jetPattern)};

    fallingRockSimulator.simulate();

    return fallingRockSimulator.getTowerHeight();
}

}
}
}