#include "Day25-Snowverload.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <unordered_map>
#include <unordered_set>
#include <queue>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const int GRAPH_EDGE_CONNECTIVITY = 3;

}

namespace AdventOfCode
{
namespace Year2023
{
namespace Day25
{

struct Edge;
using EdgePtrs = std::vector<Edge*>;
using EdgeSharedPtr = std::shared_ptr<Edge>;
using EdgeSharedPtrs = std::vector<EdgeSharedPtr>;
using NodeNameToEdgeSharedPtrs = std::unordered_map<std::string, EdgeSharedPtrs>;

struct Edge
{
    std::string sourceNodeName;
    std::string destinationNodeName;
    int capacity = 1;
    int flow = 0;
    Edge* reverse = nullptr;

    Edge(std::string sourceNodeName, std::string destinationNodeName)
        : sourceNodeName{std::move(sourceNodeName)}
        , destinationNodeName{std::move(destinationNodeName)}
    {

    }

    bool hasFreeCapacity() const
    {
        return capacity - flow > 0;
    }
};

class ComponentGraphAnalyzer
{
public:
    ComponentGraphAnalyzer(NodeNameToEdgeSharedPtrs nodeNameToEdgeSharedPtrs, std::string startNodeName, std::string endNodeName)
        : m_nodeNameToEdgeSharedPtrs{std::move(nodeNameToEdgeSharedPtrs)}
        , m_startNodeName{std::move(startNodeName)}
        , m_endNodeName{std::move(endNodeName)}
    {

    }

    void determineMaxFlow()
    {
        int maxFlowValue = 0;

        while (true)
        {
            EdgePtrs augmentingPath = createAugmentingPath();
            if (augmentingPath.empty())
            {
                break;
            }

            processAugmentingPath(augmentingPath);

            ++maxFlowValue;
        }

        m_maxFlowValue = maxFlowValue;
    }

    int getMaxFlowValue() const
    {
        return m_maxFlowValue;
    }

    int getMinCutPartitionSizesProduct() const
    {
        return m_visitedNodeNames.size() * (m_nodeNameToEdgeSharedPtrs.size() - m_visitedNodeNames.size());
    }

private:
    struct TraversalNode
    {
        std::string name;
        EdgePtrs path;

        TraversalNode(std::string name, EdgePtrs path)
            : name{std::move(name)}
            , path{std::move(path)}
        {

        }
    };

    NodeNameToEdgeSharedPtrs m_nodeNameToEdgeSharedPtrs;
    std::string m_startNodeName;
    std::string m_endNodeName;

    std::unordered_set<std::string> m_visitedNodeNames;
    int m_maxFlowValue = 0;

    EdgePtrs createAugmentingPath()
    {
        std::queue<TraversalNode> queue;
        queue.emplace(m_startNodeName, EdgePtrs{});

        m_visitedNodeNames.clear();
        m_visitedNodeNames.insert(m_startNodeName);

        while (!queue.empty())
        {
            TraversalNode currentTraversalNode = queue.front();
            queue.pop();

            if (currentTraversalNode.name == m_endNodeName)
            {
                return currentTraversalNode.path;
            }

            const EdgeSharedPtrs& neighborEdgeSharedPtrs = m_nodeNameToEdgeSharedPtrs.at(currentTraversalNode.name);

            for (const EdgeSharedPtr& neighborEdgeSharedPtr : neighborEdgeSharedPtrs)
            {
                if (neighborEdgeSharedPtr->hasFreeCapacity() && m_visitedNodeNames.insert(neighborEdgeSharedPtr->destinationNodeName).second)
                {
                    EdgePtrs newPath{currentTraversalNode.path};
                    newPath.push_back(neighborEdgeSharedPtr.get());
                    queue.emplace(neighborEdgeSharedPtr->destinationNodeName, std::move(newPath));
                }
            }
        }

        return {};
    }

    void processAugmentingPath(const EdgePtrs& augmentingPath)
    {
        for (Edge* edgePtr : augmentingPath)
        {
            edgePtr->flow += 1;
            edgePtr->reverse->flow -= 1;
        }
    }
};

NodeNameToEdgeSharedPtrs parseConnectionLines(const std::vector<std::string>& connectionLines)
{
    NodeNameToEdgeSharedPtrs nodeNameToEdgeSharedPtrs;

    for (const auto& line : connectionLines)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(": "), boost::token_compress_on);

        const std::string& sourceNodeName = tokens.front();

        for (auto destinationNodeNameIter = tokens.cbegin() + 1; destinationNodeNameIter != tokens.cend(); ++destinationNodeNameIter)
        {
            const std::string& destinationNodeName = *destinationNodeNameIter;

            EdgeSharedPtr forwardsEdgeSharedPtr = std::make_shared<Edge>(sourceNodeName, destinationNodeName);
            EdgeSharedPtr backwardsEdgeSharedPtr = std::make_shared<Edge>(destinationNodeName, sourceNodeName);

            forwardsEdgeSharedPtr->reverse = backwardsEdgeSharedPtr.get();
            backwardsEdgeSharedPtr->reverse = forwardsEdgeSharedPtr.get();

            nodeNameToEdgeSharedPtrs[sourceNodeName].push_back(std::move(forwardsEdgeSharedPtr));
            nodeNameToEdgeSharedPtrs[destinationNodeName].push_back(std::move(backwardsEdgeSharedPtr));
        }
    }

    return nodeNameToEdgeSharedPtrs;
}

int componentSizesAfterCutsMultipliedTogether(const std::vector<std::string>& connectionLines)
{
    NodeNameToEdgeSharedPtrs nodeNameToEdgeSharedPtrs = parseConnectionLines(connectionLines);
    const std::string& startNodeName = nodeNameToEdgeSharedPtrs.cbegin()->first;

    for (auto endNodeNameAndEdgePtrsIter = std::next(nodeNameToEdgeSharedPtrs.cbegin()); endNodeNameAndEdgePtrsIter != nodeNameToEdgeSharedPtrs.cend(); ++endNodeNameAndEdgePtrsIter)
    {
        const std::string& endNodeName = endNodeNameAndEdgePtrsIter->first;

        ComponentGraphAnalyzer componentGraphAnalyzer{nodeNameToEdgeSharedPtrs, startNodeName, endNodeName};
        componentGraphAnalyzer.determineMaxFlow();

        if (componentGraphAnalyzer.getMaxFlowValue() == GRAPH_EDGE_CONNECTIVITY)
        {
            return componentGraphAnalyzer.getMinCutPartitionSizesProduct();
        }
    }

    throw std::runtime_error("No solution found.");
}

}
}
}