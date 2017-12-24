#include "BridgeGraph.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

BridgeGraph::BridgeGraph(NodeIDToNeighbors nodeIDToNeighbors) noexcept
    : m_nodeIDToNeighbors{std::move(nodeIDToNeighbors)}
{

}

unsigned BridgeGraph::maxCostWalk() const
{
    unsigned maxCost = 0;

    maxCostWalkRec(0, {}, 0, maxCost);

    return maxCost;
}

void BridgeGraph::maxCostWalkRec(NodeIDType currentNodeID, const VisitedEdgeSet& visitedEdgeSet, unsigned currentCost, unsigned& maxCost) const
{
    NodeIDVector neighborIDs = m_nodeIDToNeighbors[currentNodeID];

    for (const NodeIDType neighborID : neighborIDs)
    {
        const Edge e{std::min(currentNodeID, neighborID), std::max(currentNodeID, neighborID)};

        if (visitedEdgeSet.find(e) == visitedEdgeSet.cend())
        {
            auto visitedEdgeSetExtended{visitedEdgeSet};
            visitedEdgeSetExtended.insert(e);

            const unsigned newCurrentCost = currentCost + currentNodeID + neighborID;
            maxCost = std::max(maxCost, newCurrentCost);

            maxCostWalkRec(neighborID, visitedEdgeSetExtended, newCurrentCost, maxCost);
        }
    }
}

}
