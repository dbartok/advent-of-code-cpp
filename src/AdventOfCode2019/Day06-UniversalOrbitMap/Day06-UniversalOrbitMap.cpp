#include "Day06-UniversalOrbitMap.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <unordered_set>
#include <unordered_map>
#include <queue>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using NodeIDType = std::string;
using NodeIDs = std::vector<NodeIDType>;

enum class EdgeType
{
    DIRECTED,
    UNDIRECTED,
};

class OrbitMap
{
public:
    void addEdge(NodeIDType from, NodeIDType to, EdgeType edgeType)
    {
        if (edgeType == EdgeType::UNDIRECTED)
        {
            m_nodeIDToAdjacentNodeIDs[to].push_back(from);
        }

        m_nodeIDToAdjacentNodeIDs[std::move(from)].push_back(std::move(to));
    }

    unsigned getNumTransitiveOrbits() const
    {
        return getNumTransitiveOrbitsRecursive(CENTER_OF_MASS, 0);
    }

    unsigned getDistanceBetweenStartAndGoal() const
    {
        std::unordered_set<NodeIDType> visited;
        visited.insert(START);

        std::queue<NodeIDAndDistance> nextNodeQueue;
        nextNodeQueue.push({START, 0});

        while (!nextNodeQueue.empty())
        {
            NodeIDAndDistance currentNodeIDAndDistance = nextNodeQueue.front();
            nextNodeQueue.pop();

            if (currentNodeIDAndDistance.nodeID == GOAL)
            {
                return currentNodeIDAndDistance.distance;
            }

            for (const auto& neighborNodeID : m_nodeIDToAdjacentNodeIDs.at(currentNodeIDAndDistance.nodeID))
            {
                if (visited.find(neighborNodeID) == visited.cend())
                {
                    visited.insert(neighborNodeID);
                    nextNodeQueue.push({neighborNodeID, currentNodeIDAndDistance.distance + 1});
                }
            }
        }

        throw std::runtime_error("No path found");
    }

private:
    const NodeIDType CENTER_OF_MASS = "COM";
    const NodeIDType START = "YOU";
    const NodeIDType GOAL = "SAN";

    struct NodeIDAndDistance
    {
        NodeIDType nodeID;
        unsigned distance;
    };

    std::unordered_map<NodeIDType, NodeIDs> m_nodeIDToAdjacentNodeIDs;

    unsigned getNumTransitiveOrbitsRecursive(const NodeIDType& startNodeID, unsigned numAncestors) const
    {
        const auto adjacentNodeIDsFindResult = m_nodeIDToAdjacentNodeIDs.find(startNodeID);

        if (adjacentNodeIDsFindResult == m_nodeIDToAdjacentNodeIDs.cend())
        {
            return numAncestors;
        }

        unsigned numTransitiveOrbitsFromThisNode = 0;

        for (const auto& adjacentNode : adjacentNodeIDsFindResult->second)
        {
            numTransitiveOrbitsFromThisNode += getNumTransitiveOrbitsRecursive(adjacentNode, numAncestors + 1);
        }

        return numAncestors + numTransitiveOrbitsFromThisNode;
    }
};

OrbitMap parseOrbitMap(const std::vector<std::string>& orbitLines, EdgeType edgeType)
{
    OrbitMap orbitMap;

    for (const auto& line : orbitLines)
    {
        std::vector<std::string> fromNodeIDAndToNodeID;
        Utils::splitStringIntoTypedVector(line, fromNodeIDAndToNodeID, ')');

        if (fromNodeIDAndToNodeID.size() != 2)
        {
            throw std::runtime_error("Invalid line: " + line);
        }

        orbitMap.addEdge(fromNodeIDAndToNodeID.front(), fromNodeIDAndToNodeID.back(), edgeType);
    }

    return orbitMap;
}

unsigned totalNumberOfDirectAndIndirectOrbits(const std::vector<std::string>& orbitLines)
{
    OrbitMap orbitMap = parseOrbitMap(orbitLines, EdgeType::DIRECTED);

    return orbitMap.getNumTransitiveOrbits();
}

unsigned minimumNumberOfOrbitalTransfers(const std::vector<std::string>& orbitLines)
{
    OrbitMap orbitMap = parseOrbitMap(orbitLines, EdgeType::UNDIRECTED);

    return orbitMap.getDistanceBetweenStartAndGoal() - 2;
}

}
