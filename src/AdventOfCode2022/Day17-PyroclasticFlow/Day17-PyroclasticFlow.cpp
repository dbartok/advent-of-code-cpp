#include "Day17-PyroclasticFlow.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <vector>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const int CHAMBER_WIDTH = 7;
const int ROCK_SPAWN_OFFSET_FROM_LEFT_OF_CHAMBER = 2;
const int ROCK_SPAWN_OFFSET_FROM_TOP_OF_ROCKS = 4;
const unsigned NUM_ROCKS_FIRST_PART = 2022;
const uint64_t NUM_ROCKS_SECOND_PART = 1'000'000'000'000;
const unsigned NUM_ROCK_TYPES = 5;

const unsigned MAX_NUM_ROCKS_TO_SIMULATE_BEFORE_REPETITION = 5000;
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
    FallingRockSimulator(std::vector<Direction> jetPattern, uint64_t numRocksToSimulate)
        : m_jetPattern{std::move(jetPattern)}
        , m_numRocksToSimulate{numRocksToSimulate}
    {

    }

    void simulate()
    {
        for (size_t numRocksSettled = 0; ; ++numRocksSettled)
        {
            const int maxY = getMaxYCoordinate();
            const int towerHeight = maxY + 1;
            m_numRocksSettledToTowerHeight.push_back(towerHeight);

            std::array<int, CHAMBER_WIDTH> highestPointYCoordinatesRelativeToHeight = getHighestPointYCoordinatesRelativeToHeight();
            const RockType nextRockType = static_cast<RockType>(numRocksSettled % NUM_ROCK_TYPES);
            const unsigned jetPatternIndex = m_numPushesSimulated % m_jetPattern.size();

            State currentState{std::move(highestPointYCoordinatesRelativeToHeight), nextRockType, jetPatternIndex, numRocksSettled};
            const auto insertionResult = m_seenStates.insert(currentState);

            // Cycle found
            if (!insertionResult.second)
            {
                m_towerHeight = determineHeightAtTheEndOfSimulation(*insertionResult.first, currentState);
                return;
            }

            const int spawnY = maxY + ROCK_SPAWN_OFFSET_FROM_TOP_OF_ROCKS;
            Rock rock{nextRockType, {ROCK_SPAWN_OFFSET_FROM_LEFT_OF_CHAMBER, spawnY}};

            settleRock(std::move(rock));
        }
    }

    int64_t getTowerHeight() const
    {
        return m_towerHeight;
    }

private:
    struct State
    {
        std::array<int, CHAMBER_WIDTH> highestPointYCoordinatesRelativeToHeight;
        RockType nextRock;
        unsigned jetPatternIndex;

        unsigned numRocksSettled;

        bool operator==(const State& other) const
        {
            return highestPointYCoordinatesRelativeToHeight == other.highestPointYCoordinatesRelativeToHeight &&
                nextRock == other.nextRock &&
                jetPatternIndex && other.jetPatternIndex;

        }
    };

    struct StateHash
    {
        std::size_t operator()(const State& state) const
        {
            std::size_t seed = 0;

            boost::hash_combine(seed, state.highestPointYCoordinatesRelativeToHeight);
            boost::hash_combine(seed, state.nextRock);
            boost::hash_combine(seed, state.jetPatternIndex);

            return seed;
        }
    };

    std::vector<Direction> m_jetPattern;
    uint64_t m_numRocksToSimulate;

    CoordinatesSet m_occupiedPositions;
    size_t m_numPushesSimulated = 0;
    std::unordered_set<State, StateHash> m_seenStates;
    std::vector<int> m_numRocksSettledToTowerHeight;

    int64_t m_towerHeight;

    void settleRock(Rock rock)
    {
        do
        {
            const Direction nextJetDirection = m_jetPattern.at(m_numPushesSimulated++ % m_jetPattern.size());
            rock.applyJet(nextJetDirection, m_occupiedPositions);
        } while (rock.applyFallUnit(m_occupiedPositions));

        const auto& allRockCoordinates = rock.getAllCoordinates();

        m_occupiedPositions.insert(allRockCoordinates.cbegin(), allRockCoordinates.cend());
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

    std::array<int, CHAMBER_WIDTH> getHighestPointYCoordinatesRelativeToHeight() const
    {
        std::array<int, CHAMBER_WIDTH> highestPointYCoordinatesRelativeToHeight;
        highestPointYCoordinatesRelativeToHeight.fill(std::numeric_limits<int>::min());

        for (const auto& coordinates : m_occupiedPositions)
        {
            highestPointYCoordinatesRelativeToHeight.at(coordinates.first) = std::max(highestPointYCoordinatesRelativeToHeight.at(coordinates.first), coordinates.second);
        }

        const int maxY = *std::max_element(highestPointYCoordinatesRelativeToHeight.cbegin(), highestPointYCoordinatesRelativeToHeight.cend());

        for (auto& coordinate : highestPointYCoordinatesRelativeToHeight)
        {
            coordinate -= maxY;
        }

        return highestPointYCoordinatesRelativeToHeight;
    }

    int64_t determineHeightAtTheEndOfSimulation(const State& cycleStartState, const State& cycleEndState) const
    {
        const unsigned cycleStartNumRocksSettled = cycleStartState.numRocksSettled;
        const unsigned cycleEndNumRocksSettled = cycleEndState.numRocksSettled;
        const unsigned cycleLength = cycleEndNumRocksSettled - cycleStartNumRocksSettled;

        const int cycleStartTowerHeight = m_numRocksSettledToTowerHeight.at(cycleStartNumRocksSettled);
        const int cycleEndTowerHeight = m_numRocksSettledToTowerHeight.at(cycleEndNumRocksSettled);
        const int cycleTowerHeightDelta = cycleEndTowerHeight - cycleStartTowerHeight;

        const uint64_t numRocksToSimulateAfterCycleStart = m_numRocksToSimulate - cycleStartNumRocksSettled;
        const uint64_t numFullCyclesInSimulation = numRocksToSimulateAfterCycleStart / cycleLength;

        const uint64_t finalCycleOffsetNumRocksSettledInSimulation = numRocksToSimulateAfterCycleStart % cycleLength;

        // The result here is actually the sum of two height deltas: the delta before the cycle starts, and the delta after the final cycle ends
        const int finalCycleOffsetTowerHeight = m_numRocksSettledToTowerHeight.at(cycleStartNumRocksSettled + finalCycleOffsetNumRocksSettledInSimulation);

        return numFullCyclesInSimulation * cycleTowerHeightDelta + finalCycleOffsetTowerHeight;
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

int64_t getTowerHeightForGivenNumRocks(const std::string& jetPatternString, uint64_t numRocks)
{
    std::vector<Direction> jetPattern = parseJetPatternString(jetPatternString);

    FallingRockSimulator fallingRockSimulator{std::move(jetPattern), numRocks};

    fallingRockSimulator.simulate();

    return fallingRockSimulator.getTowerHeight();
}

int64_t towerHeightAfterRocksStoppedFalling(const std::string& jetPatternString)
{
    return getTowerHeightForGivenNumRocks(jetPatternString, NUM_ROCKS_FIRST_PART);
}

int64_t towerHeightAfterRocksStoppedFallingWithManyRocks(const std::string& jetPatternString)
{
    return getTowerHeightForGivenNumRocks(jetPatternString, NUM_ROCKS_SECOND_PART);
}

}
}
}