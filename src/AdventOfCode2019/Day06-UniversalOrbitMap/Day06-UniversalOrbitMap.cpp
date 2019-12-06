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

class OrbitMap
{
public:
    void addEdge(NodeIDType from, NodeIDType to)
    {
        m_nodeIDToAdjacentNodeIDs[std::move(from)].push_back(std::move(to));
    }

    unsigned getNumTransitiveOrbits() const
    {
        return getNumTransitiveOrbitsRecursive(CENTER_OF_MASS, 0);
    }

private:
    const NodeIDType CENTER_OF_MASS = "COM";

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

OrbitMap parseOrbitMap(const std::vector<std::string>& orbitLines)
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

        orbitMap.addEdge(fromNodeIDAndToNodeID.front(), fromNodeIDAndToNodeID.back());
    }

    return orbitMap;
}

unsigned totalNumberOfDirectAndIndirectOrbits(const std::vector<std::string>& orbitLines)
{
    OrbitMap orbitMap = parseOrbitMap(orbitLines);

    return orbitMap.getNumTransitiveOrbits();
}

}
