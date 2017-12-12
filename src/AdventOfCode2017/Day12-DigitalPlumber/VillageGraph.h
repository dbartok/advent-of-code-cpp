#pragma once

#include <vector>
#include <unordered_set>

namespace AdventOfCode
{

class VillageGraph
{
public:
    using NodeIDType = unsigned;

    // Neighbors are stored as unordered set for efficiency
    using NodeIDSet = std::unordered_set<NodeIDType>;

    // Neighbors are mapped to the nodes by their position in the vector
    using NeighborSetToNodeID = std::vector<NodeIDSet>;

    VillageGraph(NeighborSetToNodeID neighborSets);
    unsigned numNodesInGroupContainingNode(NodeIDType nodeID) const;

    static VillageGraph fromNeighborsLines(const std::vector<std::string>& neighborsLines);

private:
    NeighborSetToNodeID m_neighborSets;

    void dfsRecordVisitedNodes(NodeIDType startNodeID, NodeIDSet& visitedNodeIDs) const;    
};

}
