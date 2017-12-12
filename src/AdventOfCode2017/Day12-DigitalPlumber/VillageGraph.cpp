#include "VillageGraph.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace AdventOfCode
{

VillageGraph::VillageGraph(NeighborSetToNodeID neighborSets)
    : m_neighborSets(std::move(neighborSets))
{

}

unsigned VillageGraph::numNodesInGroupContainingNode(NodeIDType nodeID) const
{
    NodeIDSet visitedNodeIDs{nodeID};
    dfsRecordVisitedNodes(nodeID, visitedNodeIDs);

    return visitedNodeIDs.size();
}

VillageGraph VillageGraph::fromNeighborsLines(const std::vector<std::string>& neighborsLines)
{
    NodeIDType expectedNodeID = 0;
    NeighborSetToNodeID neighborSets;
    for (const auto& neighborsLine : neighborsLines)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, neighborsLine, boost::is_any_of(", "), boost::algorithm::token_compress_on);

        if (tokens.size() < 2)
        {
            throw std::runtime_error("Each line needs to have at least 2 tokens.");
        }

        NodeIDType nodeID = boost::lexical_cast<NodeIDType>(tokens[0]);
        if (nodeID != expectedNodeID++)
        {
            throw std::runtime_error("Neighbor lines are expected to be in order.");
        }

        std::vector<NodeIDType> neighbors;
        std::transform(tokens.cbegin() + 2, tokens.cend(), std::back_inserter(neighbors), [](const std::string& nodeIDString)
        {
            return boost::lexical_cast<NodeIDType>(nodeIDString);
        });

        NodeIDSet neighborSet{std::make_move_iterator(neighbors.begin()), std::make_move_iterator(neighbors.end())};
        neighborSets.push_back(std::move(neighborSet));
    }

    return VillageGraph{std::move(neighborSets)};
}

void VillageGraph::dfsRecordVisitedNodes(NodeIDType startNodeID, NodeIDSet& visitedNodeIDs) const
{
    for (const auto& neighborNodeID : m_neighborSets[startNodeID])
    {
        if (visitedNodeIDs.find(neighborNodeID) == visitedNodeIDs.end())
        {
            visitedNodeIDs.insert(neighborNodeID);
            dfsRecordVisitedNodes(neighborNodeID, visitedNodeIDs);
        }
    }
}

}
