#include "Day16-ProboscideaVolcanium.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>

#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <bitset>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const char* START_ROOM_NAME = "AA";
const size_t START_ROOM_INDEX = 0;
const int AVAILABLE_TIME_FIRST_PART = 30;
const int AVAILABLE_TIME_SECOND_PART = 26;
const size_t MAX_NUM_RELEVANT_ROOMS = 75;

}

namespace AdventOfCode
{
namespace Year2022
{
namespace Day16
{

struct ValveRoom
{
    std::string name;
    int flowRate;
    std::vector<std::string> connectedRoomNames;

};

using CostMatrixRow = std::vector<int>;
using CostMatrix = std::vector<CostMatrixRow>;

class ValveNetworkSimplifier
{
public:
    ValveNetworkSimplifier(std::vector<ValveRoom> valveRooms)
    {
        for (auto& room : valveRooms)
        {
            m_roomNameToRoom.emplace(room.name, std::move(room));
        }
    }

    void buildCostMatrix()
    {
        allocateCostMatrixIndices();

        const size_t numRooms = m_roomNameToRoom.size();

        m_costMatrix = CostMatrix(numRooms, CostMatrixRow(numRooms));

        for (const auto& roomNameAndRoom : m_roomNameToRoom)
        {
            buildCostMatrixRow(roomNameAndRoom.first);
        }
    }

    CostMatrix getCostMatrix() const
    {
        return m_costMatrix;
    }

    std::unordered_map<size_t, int> getRoomIndexToFlowRate() const
    {
        std::unordered_map<size_t, int> roomIndexToFlowRate;

        for (const auto& roomNameAndRoom : m_roomNameToRoom)
        {
            const size_t roomIndex = m_roomNameToCostMatrixIndex.at(roomNameAndRoom.first);
            roomIndexToFlowRate.emplace(roomIndex, roomNameAndRoom.second.flowRate);
        }

        return roomIndexToFlowRate;
    }

private:
    std::unordered_map<std::string, ValveRoom> m_roomNameToRoom;

    std::unordered_map<std::string, size_t> m_roomNameToCostMatrixIndex;
    CostMatrix m_costMatrix;

    void allocateCostMatrixIndices()
    {
        m_roomNameToCostMatrixIndex.emplace(START_ROOM_NAME, START_ROOM_INDEX);

        size_t currentIndex = 1;

        for (const auto& roomNameAndRoom : m_roomNameToRoom)
        {
            const bool wasInserted = m_roomNameToCostMatrixIndex.emplace(roomNameAndRoom.first, currentIndex).second;
            if (wasInserted)
            {
                ++currentIndex;
            }
        }
    }

    void buildCostMatrixRow(const std::string& startRoomName)
    {
        const size_t startRoomIndex = m_roomNameToCostMatrixIndex.at(startRoomName);

        std::queue<std::string> roomNamesBfsQueue;
        roomNamesBfsQueue.push(startRoomName);

        while (!roomNamesBfsQueue.empty())
        {
            std::string currentRoomName = roomNamesBfsQueue.front();
            roomNamesBfsQueue.pop();

            const auto& currentRoom = m_roomNameToRoom.at(currentRoomName);
            const size_t currentRoomIndex = m_roomNameToCostMatrixIndex.at(currentRoomName);
            const int currentRoomCost = m_costMatrix.at(startRoomIndex).at(currentRoomIndex);

            for (const auto& connectedRoomName : currentRoom.connectedRoomNames)
            {
                const size_t connectedRoomIndex = m_roomNameToCostMatrixIndex.at(connectedRoomName);

                if (startRoomIndex != connectedRoomIndex && m_costMatrix.at(startRoomIndex).at(connectedRoomIndex) == 0)
                {
                    m_costMatrix.at(startRoomIndex).at(connectedRoomIndex) = currentRoomCost + 1;
                    roomNamesBfsQueue.push(connectedRoomName);
                }
            }
        }
    }
};

class FlowRateOptimizer
{
public:
    FlowRateOptimizer(CostMatrix costMatrix, std::unordered_map<size_t, int> roomIndexToFlowRate, int availableTime)
        : m_costMatrix{std::move(costMatrix)}
        , m_roomIndexToFlowRate{std::move(roomIndexToFlowRate)}
        , m_availableTime{availableTime}
    {
        for (const auto& roomIndexAndFlowRate : m_roomIndexToFlowRate)
        {
            if (roomIndexAndFlowRate.second != 0)
            {
                m_roomIndicesWithNonZeroFlowRate.push_back(roomIndexAndFlowRate.first);
            }
        }
    }

    void createAllStates()
    {
        StateSet openStates;
        State startState{START_ROOM_INDEX, m_availableTime, {}, 0};
        openStates.insert(startState);

        while (!openStates.empty())
        {
            StateSet nextOpenStates;

            for (const auto& currentState : openStates)
            {
                std::vector<State> nextStates = getNextStates(currentState);

                for (const auto& nextState : nextStates)
                {
                    insertKeepingHigherTotalPressureReleased(nextOpenStates, nextState);
                    insertKeepingHigherTotalPressureReleased(m_allSeenStates, nextState);
                }
            }

            openStates = nextOpenStates;
        }
    }

    int getMaxPressureReleasedWithOneWorker() const
    {
        const auto maxPressureReleasedStateIter = std::max_element(m_allSeenStates.cbegin(), m_allSeenStates.cend(), [](const auto& lhs, const auto& rhs)
                                                                   {
                                                                       return lhs.totalPressureReleased < rhs.totalPressureReleased;
                                                                   });

        return maxPressureReleasedStateIter->totalPressureReleased;
    }

    int getMaxPressureReleasedWithTwoWorkers() const
    {
        int maxPressureReleased = 0;

        for (const auto& firstWorkerState : m_allSeenStates)
        {
            for (const auto& secondWorkerState : m_allSeenStates)
            {
                const auto visitedRoomIntersection = firstWorkerState.visitedRoomIndices & secondWorkerState.visitedRoomIndices;
                if (visitedRoomIntersection.none())
                {
                    const int pressureReleased = firstWorkerState.totalPressureReleased + secondWorkerState.totalPressureReleased;
                    maxPressureReleased = std::max(maxPressureReleased, pressureReleased);
                }
            }
        }

        return maxPressureReleased;
    }

private:
    struct State
    {
        size_t currentRoomIndex;
        int remainingTime;
        std::bitset<MAX_NUM_RELEVANT_ROOMS> visitedRoomIndices;

        int totalPressureReleased;

        bool operator==(const State& other) const
        {
            return currentRoomIndex == other.currentRoomIndex &&
                remainingTime == other.remainingTime &&
                visitedRoomIndices == other.visitedRoomIndices;
        }
    };

    struct StateHash
    {
        std::size_t operator()(const State& state) const
        {
            std::size_t seed = 0;

            boost::hash_combine(seed, state.currentRoomIndex);
            boost::hash_combine(seed, state.remainingTime);
            boost::hash_combine(seed, state.visitedRoomIndices.to_ullong());

            return seed;
        }
    };

    using StateSet = std::unordered_set<State, StateHash>;

    CostMatrix m_costMatrix;
    std::unordered_map<size_t, int> m_roomIndexToFlowRate;
    int m_availableTime;
    std::vector<size_t> m_roomIndicesWithNonZeroFlowRate;

    StateSet m_allSeenStates;

    std::vector<State> getNextStates(const State& currentState) const
    {
        std::vector<State> nextStates;

        for (const size_t nextRoomIndex : m_roomIndicesWithNonZeroFlowRate)
        {
            if (currentState.visitedRoomIndices.test(nextRoomIndex))
            {
                continue;
            }

            const int distanceToNextRoom = m_costMatrix.at(currentState.currentRoomIndex).at(nextRoomIndex);
            const int remainingTimeAfterNextValveOpen = currentState.remainingTime - distanceToNextRoom - 1;

            // No time for any newly opened valve to release pressure
            if (remainingTimeAfterNextValveOpen < 1)
            {
                continue;
            }

            auto newVisitedRoomIndices{currentState.visitedRoomIndices};
            newVisitedRoomIndices.set(nextRoomIndex);

            const int currentRoomFlowRate = m_roomIndexToFlowRate.at(nextRoomIndex);
            const int newTotalPressureReleased = currentState.totalPressureReleased + remainingTimeAfterNextValveOpen * currentRoomFlowRate;

            State nextState{nextRoomIndex, remainingTimeAfterNextValveOpen, newVisitedRoomIndices, newTotalPressureReleased};

            nextStates.push_back(std::move(nextState));
        }

        return nextStates;
    }

    static void insertKeepingHigherTotalPressureReleased(StateSet& states, State newState)
    {
        auto insertionResult = states.insert(newState);
        if (!insertionResult.second)
        {
            auto existingStateIter = insertionResult.first;
            if (newState.totalPressureReleased > existingStateIter->totalPressureReleased)
            {
                states.erase(existingStateIter);
                states.insert(std::move(newState));
            }
        }
    }
};

ValveRoom parseValveRoomLine(const std::string& valveRoomLine)
{
    const std::regex valveRoomLineRegex(R"(Valve ([A-Z]+) has flow rate=([0-9]+); tunnels? leads? to valves? ([A-Z,\s]+))");
    std::smatch valveRoomLineMatch;

    std::regex_match(valveRoomLine, valveRoomLineMatch, valveRoomLineRegex);

    std::string name = valveRoomLineMatch[1];
    const int flowRate = std::stoi(valveRoomLineMatch[2]);

    std::vector<std::string> connectedRoomNames;
    boost::split(connectedRoomNames, std::string{valveRoomLineMatch[3]}, boost::is_any_of(" ,"), boost::token_compress_on);

    return ValveRoom{std::move(name), flowRate, std::move(connectedRoomNames)};
}

std::vector<ValveRoom> parseValveRoomLines(const std::vector<std::string>& valveRoomLines)
{
    std::vector<ValveRoom> valveRooms;

    for (const auto& valveRoomLine : valveRoomLines)
    {
        ValveRoom valveRoom = parseValveRoomLine(valveRoomLine);
        valveRooms.push_back(std::move(valveRoom));
    }

    return valveRooms;
}

FlowRateOptimizer createFlowRateOptimizer(const std::vector<std::string>& valveRoomLines, int availableTime)
{
    std::vector<ValveRoom> valveRooms = parseValveRoomLines(valveRoomLines);

    ValveNetworkSimplifier valveNetworkSimplifier{std::move(valveRooms)};
    valveNetworkSimplifier.buildCostMatrix();

    CostMatrix costMatrix = valveNetworkSimplifier.getCostMatrix();
    std::unordered_map<size_t, int> roomIndexToFlowRate = valveNetworkSimplifier.getRoomIndexToFlowRate();

    FlowRateOptimizer flowRateOptimizer{std::move(costMatrix), std::move(roomIndexToFlowRate), availableTime};
    return flowRateOptimizer;
}

int mostPressureThatCanBeReleased(const std::vector<std::string>& valveRoomLines)
{
    FlowRateOptimizer flowRateOptimizer = createFlowRateOptimizer(valveRoomLines, AVAILABLE_TIME_FIRST_PART);

    flowRateOptimizer.createAllStates();

    return flowRateOptimizer.getMaxPressureReleasedWithOneWorker();
}

int mostPressureThatCanBeReleasedWorkingWithElephant(const std::vector<std::string>& valveRoomLines)
{
    FlowRateOptimizer flowRateOptimizer = createFlowRateOptimizer(valveRoomLines, AVAILABLE_TIME_SECOND_PART);
    
    flowRateOptimizer.createAllStates();

    return flowRateOptimizer.getMaxPressureReleasedWithTwoWorkers();
}

}
}
}