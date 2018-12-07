#include "Day07-TheSumOfItsParts.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <regex>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <queue>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using NodeIDType = char;
using OrderedNodes = std::set<NodeIDType, std::greater<NodeIDType>>;
using UnorderedNodes = std::unordered_set<NodeIDType>;

class Worker
{
public:
    Worker()
        : m_isIdle(true)
        , m_nodeID{'\0'}
        , m_remainingDuration{0}
    {

    }

    bool isIdle() const
    {
        return m_isIdle;
    }

    bool isFinished() const
    {
        return !isIdle() && m_remainingDuration == 0;
    }

    const NodeIDType& getNodeID() const
    {
        return m_nodeID;
    }

    void startWork(NodeIDType nodeID, unsigned duration)
    {
        m_isIdle = false;
        m_nodeID = std::move(nodeID);
        m_remainingDuration = duration;
    }

    void startIdling()
    {
        m_isIdle = true;
    }

    void advanceCycle()
    {
        if (!isIdle())
        {
            --m_remainingDuration;
        }
    }

private:
    bool m_isIdle;
    NodeIDType m_nodeID;
    unsigned m_remainingDuration;
};

class DependencyGraph
{
public:
    void addEdge(NodeIDType from, NodeIDType to)
    {
        m_allNodesOrdered.insert(from);
        m_allNodesOrdered.insert(to);

        // Register the destination node to make sure we have a complete collection of all nodes
        m_nodeIDToAdjacentNodes.emplace(to, OrderedNodes{});
        m_nodeIDToAdjacentNodes[std::move(from)].insert(std::move(to));
    }

    std::string getTopologicalOrder()
    {
        std::vector<NodeIDType> finishedNodes;
        std::unordered_set<NodeIDType> visitedNodes;

        for (const auto& nodeID : m_allNodesOrdered)
        {
            if (visitedNodes.find(nodeID) == visitedNodes.end())
            {
                dfsFillOutFinishedNodes(nodeID, visitedNodes, finishedNodes);
            }
        }

        std::string topologicalOrder{std::make_move_iterator(finishedNodes.rbegin()), std::make_move_iterator(finishedNodes.rend())};

        return topologicalOrder;
    }

    unsigned getTimeUntilAllStepsAreDone(unsigned numWorkers, unsigned additionalDuration)
    {
        std::unordered_map<NodeIDType, UnorderedNodes> m_nodeIDsToDependencyNodeIDs = getReversedEdges();

        std::set<NodeIDType> nodeIDsReadyToStart = getStartingNodeIDs();
        std::vector<Worker> workers(numWorkers, Worker{});

        unsigned numCycles = 0;

        while (!allWorkersIdle(workers) || !nodeIDsReadyToStart.empty())
        {
            for (auto& worker : workers)
            {
                if (worker.isIdle())
                {
                    if (!nodeIDsReadyToStart.empty())
                    {
                        NodeIDType nextNodeID = *nodeIDsReadyToStart.begin();
                        unsigned nextDuration = nextNodeID - 'A' + 1 + additionalDuration;

                        worker.startWork(nextNodeID, nextDuration);
                        nodeIDsReadyToStart.erase(nodeIDsReadyToStart.begin());
                    }
                }
            }

            for (auto& worker : workers)
            {
                worker.advanceCycle();

                if (worker.isFinished())
                {
                    for (const auto& dependedOnByNodeID : m_nodeIDToAdjacentNodes.at(worker.getNodeID()))
                    {
                        auto& dependencies = m_nodeIDsToDependencyNodeIDs.at(dependedOnByNodeID);

                        bool wasErased = dependencies.erase(worker.getNodeID());
                        assert(wasErased);

                        if (dependencies.empty())
                        {
                            nodeIDsReadyToStart.insert(dependedOnByNodeID);
                        }
                    }

                    worker.startIdling();
                }
            }

            ++numCycles;
        }

        return numCycles;
    }

private:
    std::unordered_map<NodeIDType, OrderedNodes> m_nodeIDToAdjacentNodes;
    OrderedNodes m_allNodesOrdered;

    void dfsFillOutFinishedNodes(const NodeIDType& startNodeID, std::unordered_set<NodeIDType>& visitedNodes, std::vector<NodeIDType>& finishedNodes)
    {
        visitedNodes.insert(startNodeID);

        for (const auto& neighborID : m_nodeIDToAdjacentNodes.at(startNodeID))
        {
            if (visitedNodes.find(neighborID) == visitedNodes.end())
            {
                dfsFillOutFinishedNodes(neighborID, visitedNodes, finishedNodes);
            }
        }

        finishedNodes.push_back(startNodeID);
    }

    std::set<NodeIDType> getStartingNodeIDs()
    {
        std::set<NodeIDType> startingNodeIDs{m_allNodesOrdered.cbegin(), m_allNodesOrdered.cend()};

        // Remove all nodes which have inbound edges
        for (const auto& startNodeId : m_allNodesOrdered)
        {
            for (const auto& neighorId : m_nodeIDToAdjacentNodes.at(startNodeId))
            {
                startingNodeIDs.erase(neighorId);
            }
        }

        return startingNodeIDs;
    }

    std::unordered_map<NodeIDType, UnorderedNodes> getReversedEdges()
    {
        std::unordered_map<NodeIDType, UnorderedNodes> reversedEdges;

        for (const auto& startNodeId : m_allNodesOrdered)
        {
            for (const auto& neighorId : m_nodeIDToAdjacentNodes.at(startNodeId))
            {
                reversedEdges[neighorId].insert(startNodeId);
            }
        }

        return reversedEdges;
    }

    bool allWorkersIdle(const std::vector<Worker>& workers)
    {
        return std::all_of(workers.cbegin(), workers.cend(), [](const Worker& worker) {return worker.isIdle(); });
    }
};

DependencyGraph parseDependencyGraphFromInstructionLines(const std::vector<std::string>& instructionLines)
{
    DependencyGraph graph;

    for (const auto& line : instructionLines)
    {
        std::regex guardIDRegex("[Ss]tep ([A-Z])");
        std::smatch matchResults;

        std::regex_search(line, matchResults, guardIDRegex);
        NodeIDType from = matchResults[1].str().front();

        std::string suffix = matchResults.suffix();
        std::regex_search(suffix, matchResults, guardIDRegex);
        NodeIDType to = matchResults[1].str().front();

        graph.addEdge(std::move(from), std::move(to));
    }

    return graph;
}

std::string topologicalOrderOfInstructions(const std::vector<std::string>& instructionLines)
{
    DependencyGraph graph = parseDependencyGraphFromInstructionLines(instructionLines);

    return graph.getTopologicalOrder();
}

unsigned timeUntilAllStepsAreDone(const std::vector<std::string>& instructionLines, unsigned numWorkers, unsigned additionalDuration)
{
    DependencyGraph graph = parseDependencyGraphFromInstructionLines(instructionLines);

    return graph.getTimeUntilAllStepsAreDone(numWorkers, additionalDuration);
}

}
