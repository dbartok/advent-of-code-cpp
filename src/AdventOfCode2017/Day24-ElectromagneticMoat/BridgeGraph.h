#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <vector>
#include <unordered_set>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class BridgeGraph
{
public:
    using NodeIDType = unsigned;

    // Neighbors are stored as IDs in a vector
    using NodeIDVector = std::vector<NodeIDType>;

    // Neighbors are mapped to the nodes by their position in the vector
    using NodeIDToNeighbors = std::vector<NodeIDVector>;

    BridgeGraph(NodeIDToNeighbors nodeIDToNeighbors) noexcept;
    unsigned maxCostWalk() const;

private:
    NodeIDToNeighbors m_nodeIDToNeighbors;

    using Edge = std::pair<NodeIDType, NodeIDType>;
    using VisitedEdgeSet = std::unordered_set<Edge, boost::hash<Edge>>;

    void maxCostWalkRec(NodeIDType currentNodeID, const VisitedEdgeSet& visitedEdgeSet, unsigned currentCost, unsigned& maxCost) const;
};

}
