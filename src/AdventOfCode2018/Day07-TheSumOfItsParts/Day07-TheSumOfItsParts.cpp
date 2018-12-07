#include "Day07-TheSumOfItsParts.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <regex>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using NodeIDType = char;
using OrderedNodes = std::set<NodeIDType, std::greater<NodeIDType>>;

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

}
