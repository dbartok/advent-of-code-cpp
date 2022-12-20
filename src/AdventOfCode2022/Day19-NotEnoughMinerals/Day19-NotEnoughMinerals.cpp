#include "Day19-NotEnoughMinerals.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <array>
#include <regex>
#include <unordered_set>
#include <stack>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const int TOTAL_AVAILABLE_TIME_FIRST_PART = 24;
const int TOTAL_AVAILABLE_TIME_SECOND_PART = 32;
const size_t NUM_MAX_BLUEPRINTS_SECOND_PART = 3;

}

namespace AdventOfCode
{
namespace Year2022
{
namespace Day19
{

const size_t ORE_INDEX = 0;
const size_t CLAY_INDEX = 1;
const size_t OBSIDIAN_INDEX = 2;
const size_t GEODE_INDEX = 3;
const size_t NUM_MINERAL_TYPES = 4;
using MineralArray = std::array<int, NUM_MINERAL_TYPES>;
using MineralMatrix = std::array<MineralArray, NUM_MINERAL_TYPES>;

const MineralArray INITIAL_MINERAL_PRODUCTION = {1, 0, 0, 0};
const MineralArray ZERO_MINERAL_ARRAY = {0, 0, 0, 0};

class GeodeHarvestingSimulator
{
public:
    GeodeHarvestingSimulator(MineralMatrix robotCostMatrix, int totalAvailableTime)
        : m_robotCostMatrix{std::move(robotCostMatrix)}
        , m_totalAvailableTime{totalAvailableTime}
        , m_maxMineralCosts{ZERO_MINERAL_ARRAY}
    {
        for (const auto& cost : m_robotCostMatrix)
        {
            for (size_t i = 0; i < NUM_MINERAL_TYPES; ++i)
            {
                m_maxMineralCosts.at(i) = std::max(m_maxMineralCosts.at(i), cost.at(i));
            }
        }
    }

    void simulate()
    {
        std::stack<State> stateStack;
        State startState = {INITIAL_MINERAL_PRODUCTION, ZERO_MINERAL_ARRAY, m_totalAvailableTime};
        stateStack.push(startState);
        StateSet seenStates{startState};

        while (!stateStack.empty())
        {
            const State currentState = stateStack.top();
            stateStack.pop();

            std::vector<State> nextStates = getNextStates(currentState);
            std::vector<State> prunedNextStates = pruneNextStates(std::move(nextStates));

            for (auto& nextState : prunedNextStates)
            {
                m_maxNumGeodesHarvested = std::max(m_maxNumGeodesHarvested, nextState.mineralsAvailable.at(GEODE_INDEX));
                if (seenStates.insert(nextState).second)
                {
                    stateStack.push(std::move(nextState));
                }
            }
        }
    }

    int getMaxNumGeodesHarvested() const
    {
        return m_maxNumGeodesHarvested;
    }

private:
    struct State
    {
        MineralArray mineralProduction;
        MineralArray mineralsAvailable;
        int remainingTime;

        bool operator==(const State& other) const
        {
            return mineralProduction == other.mineralProduction &&
                mineralsAvailable == other.mineralsAvailable &&
                remainingTime == other.remainingTime;
        }
    };

    struct StateHash
    {
        std::size_t operator()(const State& state) const
        {
            std::size_t seed = 0;

            boost::hash_combine(seed, state.mineralProduction);
            boost::hash_combine(seed, state.mineralsAvailable);
            boost::hash_combine(seed, state.remainingTime);

            return seed;
        }
    };

    using StateSet = std::unordered_set<State, StateHash>;

    MineralMatrix m_robotCostMatrix;
    int m_totalAvailableTime;
    MineralArray m_maxMineralCosts;

    int m_maxNumGeodesHarvested = 0;

    std::vector<State> getNextStates(const State& currentState) const
    {
        if (currentState.remainingTime == 0)
        {
            return {};
        }

        std::vector<State> nextStates;

        // Build
        for (size_t robotToBuildIndex = 0; robotToBuildIndex < NUM_MINERAL_TYPES; ++robotToBuildIndex)
        {
            // No time to benefit from build
            if (currentState.remainingTime == 1 || (currentState.remainingTime == 2 && robotToBuildIndex != GEODE_INDEX))
            {
                continue;
            }

            if (canAffordToBuild(currentState, robotToBuildIndex))
            {
                auto newState{currentState};
                elapseMinute(newState);
                build(newState, robotToBuildIndex);
                nextStates.push_back(std::move(newState));
            }
        }

        // Wait
        auto newState{currentState};
        elapseMinute(newState);
        nextStates.push_back(std::move(newState));

        return nextStates;
    }

    std::vector<State> pruneNextStates(std::vector<State> nextStates) const
    {
        std::vector<State> prunedNextStates;

        std::copy_if(std::make_move_iterator(nextStates.begin()), std::make_move_iterator(nextStates.end()), std::back_inserter(prunedNextStates), [this](const State& state)
                     {
                         return !this->isOverproducing(state) && this->canStillExceedBestSoFar(state);
                     });

        return prunedNextStates;
    }

    bool canAffordToBuild(const State& state, size_t robotToBuildIndex) const
    {
        const auto& robotCost = m_robotCostMatrix.at(robotToBuildIndex);
        for (size_t i = 0; i < NUM_MINERAL_TYPES; ++i)
        {
            if (state.mineralsAvailable.at(i) < robotCost.at(i))
            {
                return false;
            }
        }

        return true;
    }

    void build(State& state, size_t robotToBuildIndex) const
    {
        const auto& robotCost = m_robotCostMatrix.at(robotToBuildIndex);
        for (size_t i = 0; i < NUM_MINERAL_TYPES; ++i)
        {
            state.mineralsAvailable.at(i) -= robotCost.at(i);
        }

        ++state.mineralProduction.at(robotToBuildIndex);
    }

    bool isOverproducing(const State& state) const
    {
        for (size_t i = 0; i < NUM_MINERAL_TYPES; ++i)
        {
            if (i == GEODE_INDEX)
            {
                continue;
            }

            if (state.mineralProduction.at(i) > m_maxMineralCosts.at(i))
            {
                return true;
            }
        }

        return false;
    }

    bool canStillExceedBestSoFar(const State& state) const
    {
        const int currentNumGeodes = state.mineralsAvailable.at(GEODE_INDEX);
        const int geodeProduction = state.mineralProduction.at(GEODE_INDEX);

        const int numGeodesAtEndWithCurrentProduction = currentNumGeodes + geodeProduction * state.remainingTime;

        const int remainingProductionTime = state.remainingTime - 1;
        const int numMaxGeodesProducedByNewRobots = remainingProductionTime * (remainingProductionTime + 1) / 2;

        return numGeodesAtEndWithCurrentProduction + numMaxGeodesProducedByNewRobots > m_maxNumGeodesHarvested;
    }

    static void elapseMinute(State& state)
    {
        for (size_t i = 0; i < NUM_MINERAL_TYPES; ++i)
        {
            state.mineralsAvailable.at(i) += state.mineralProduction.at(i);
        }

        --state.remainingTime;
    }
};

MineralMatrix parseBlueprintLine(const std::string& blueprintLine)
{
    const std::regex blueprintLineRegex(R"(Blueprint [0-9]+: Each ore robot costs ([0-9]+) ore. Each clay robot costs ([0-9]+) ore. Each obsidian robot costs ([0-9]+) ore and ([0-9]+) clay. Each geode robot costs ([0-9]+) ore and ([0-9]+) obsidian.)");
    std::smatch blueprintLineMatch;

    std::regex_match(blueprintLine, blueprintLineMatch, blueprintLineRegex);

    const int oreRobotOreCost = std::stoi(blueprintLineMatch[1]);
    const int clayRobotOreCost = std::stoi(blueprintLineMatch[2]);
    const int obsidianRobotOreCost = std::stoi(blueprintLineMatch[3]);
    const int obsidianRobotClayCost = std::stoi(blueprintLineMatch[4]);
    const int geodeRobotOreCost = std::stoi(blueprintLineMatch[5]);
    const int geodeRobotObsidianCost = std::stoi(blueprintLineMatch[6]);

    MineralMatrix robotCostMatrix{0};

    robotCostMatrix.at(ORE_INDEX).at(ORE_INDEX) = oreRobotOreCost;
    robotCostMatrix.at(CLAY_INDEX).at(ORE_INDEX) = clayRobotOreCost;
    robotCostMatrix.at(OBSIDIAN_INDEX).at(ORE_INDEX) = obsidianRobotOreCost;
    robotCostMatrix.at(OBSIDIAN_INDEX).at(CLAY_INDEX) = obsidianRobotClayCost;
    robotCostMatrix.at(GEODE_INDEX).at(ORE_INDEX) = geodeRobotOreCost;
    robotCostMatrix.at(GEODE_INDEX).at(OBSIDIAN_INDEX) = geodeRobotObsidianCost;

    return robotCostMatrix;
}

std::vector<MineralMatrix> parseBlueprintLines(const std::vector<std::string>& blueprintLines)
{
    std::vector<MineralMatrix> robotCostMatrices;

    for (const auto& blueprintLine : blueprintLines)
    {
        MineralMatrix robotCostMatrix = parseBlueprintLine(blueprintLine);
        robotCostMatrices.push_back(std::move(robotCostMatrix));
    }

    return robotCostMatrices;
}

int sumOfBlueprintQualityLevels(const std::vector<std::string>& blueprintLines)
{
    std::vector<MineralMatrix> robotCostMatrices = parseBlueprintLines(blueprintLines);
    int totalQualityLevel = 0;

    for (size_t i = 0; i < robotCostMatrices.size(); ++i)
    {
        GeodeHarvestingSimulator geodeHarvestingSimulator{std::move(robotCostMatrices.at(i)), TOTAL_AVAILABLE_TIME_FIRST_PART};
        geodeHarvestingSimulator.simulate();
        const int maxNumGeodesHarvested = geodeHarvestingSimulator.getMaxNumGeodesHarvested();
        const int qualityLevel = maxNumGeodesHarvested * (i + 1);

        totalQualityLevel += qualityLevel;
    }

    return totalQualityLevel;
}

int productOfFirstFewBlueprintMaxGeodesWithMoreTime(const std::vector<std::string>& blueprintLines)
{
    std::vector<MineralMatrix> robotCostMatrices = parseBlueprintLines(blueprintLines);
    int productOfMaxGeodes = 1;

    for (size_t i = 0; i < robotCostMatrices.size() && i < NUM_MAX_BLUEPRINTS_SECOND_PART; ++i)
    {
        GeodeHarvestingSimulator geodeHarvestingSimulator{std::move(robotCostMatrices.at(i)), TOTAL_AVAILABLE_TIME_SECOND_PART};
        geodeHarvestingSimulator.simulate();
        const int maxNumGeodesHarvested = geodeHarvestingSimulator.getMaxNumGeodesHarvested();

        productOfMaxGeodes *= maxNumGeodesHarvested;
    }

    return productOfMaxGeodes;
}

}
}
}