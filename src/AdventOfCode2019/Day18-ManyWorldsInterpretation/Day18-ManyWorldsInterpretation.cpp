#include "Day18-ManyWorldsInterpretation.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash/hash.hpp>

#include <bitset>
#include <stdexcept>
#include <queue>
#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
const size_t ALPHABET_LENGTH = 26;
}

namespace AdventOfCode
{

using VaultMap = std::vector<std::string>;

struct VaultSearchNode
{
    int x;
    int y;
    int numSteps;
    std::bitset<ALPHABET_LENGTH> keys;

    bool operator==(const VaultSearchNode& other) const
    {
        return x == other.x && y == other.y && keys == other.keys;
    }
};

struct VaultSearchNodeHash
{
    std::size_t operator()(const VaultSearchNode& node) const
    {
        std::size_t seed = 0;

        boost::hash_combine(seed, node.x);
        boost::hash_combine(seed, node.y);
        boost::hash_combine(seed, node.keys.to_ulong());

        return seed;
    }
};

class VaultTraverser
{
public:
    VaultTraverser(VaultMap vaultMap)
        : m_map{std::move(vaultMap)}
    {

    }

    void traverseAllKeys()
    {
        std::queue<VaultSearchNode> bfsQueue;
        VaultSearchNode initialNode = createInitialNode();
        int numTotalKeys = getNumTotalKeys();
        bfsQueue.push(initialNode);

        std::unordered_set<VaultSearchNode, VaultSearchNodeHash> visited;
        visited.insert(initialNode);

        while (!bfsQueue.empty())
        {
            VaultSearchNode currentNode = bfsQueue.front();
            bfsQueue.pop();

            if (currentNode.keys.count() == numTotalKeys)
            {
                m_shortestPathLength = currentNode.numSteps;
                return;
            }

            std::vector<VaultSearchNode> neighborNodes = createNeighborNodes(currentNode);

            for (auto& node : neighborNodes)
            {
                if (visited.find(node) == visited.cend())
                {
                    visited.insert(node);
                    bfsQueue.push(node);
                }
            }
        }

        throw std::runtime_error("Unable to find all keys.");
    }

    int getShortestPathLength() const
    {
        return m_shortestPathLength;
    }

private:
    VaultMap m_map;

    int m_shortestPathLength = std::numeric_limits<int>::max();

    VaultSearchNode createInitialNode()
    {
        for (int j = 0; j < m_map.size(); ++j)
        {
            for (int i = 0; i < m_map.at(j).size(); ++i)
            {
                if (m_map.at(j).at(i) == '@')
                {
                    m_map.at(j).at(i) = '.';
                    return VaultSearchNode{i, j, 0, {}};
                }
            }
        }

        throw std::runtime_error("Unable to create initial state.");
    }

    std::vector<VaultSearchNode> createNeighborNodes(const VaultSearchNode& currentNode)
    {
        std::vector<VaultSearchNode> neighborNodes;
        std::vector<VaultSearchNode> spatialNeighborNodes = createSpatialNeighborNodes(currentNode);

        for (auto& spatialNeighborNode : spatialNeighborNodes)
        {
            char tile = m_map.at(spatialNeighborNode.y).at(spatialNeighborNode.x);
            if (isKey(tile))
            {
                spatialNeighborNode.keys.set(tile - 'a');
            }

            if (!isObstructed(tile, spatialNeighborNode.keys))
            {
                neighborNodes.push_back(spatialNeighborNode);
            }
        }

        return neighborNodes;
    }

    std::vector<VaultSearchNode> createSpatialNeighborNodes(const VaultSearchNode& currentNode)
    {
        std::vector<VaultSearchNode> spatialNeighborNodes;
        spatialNeighborNodes.push_back(VaultSearchNode{currentNode.x + 1, currentNode.y, currentNode.numSteps + 1, currentNode.keys});
        spatialNeighborNodes.push_back(VaultSearchNode{currentNode.x - 1, currentNode.y, currentNode.numSteps + 1, currentNode.keys});
        spatialNeighborNodes.push_back(VaultSearchNode{currentNode.x, currentNode.y + 1, currentNode.numSteps + 1, currentNode.keys});
        spatialNeighborNodes.push_back(VaultSearchNode{currentNode.x, currentNode.y - 1, currentNode.numSteps + 1, currentNode.keys});
        return spatialNeighborNodes;
    }

    int getNumTotalKeys() const
    {
        int numTotalKeys = 0;

        for (int j = 0; j < m_map.size(); ++j)
        {
            for (int i = 0; i < m_map.at(j).size(); ++i)
            {
                if (isKey(m_map.at(j).at(i)))
                {
                    ++numTotalKeys;
                }
            }
        }

        return numTotalKeys;
    }

    bool isKey(char tile) const
    {
        return tile >= 'a' && tile <= 'z';
    }

    bool isObstructed(char tile, const std::bitset<ALPHABET_LENGTH>& keys) const
    {
        if (tile == '#')
        {
            return true;
        }

        if (isDoor(tile))
        {
            char key = getKeyForDoor(tile);
            if (!keys.test(key - 'a'))
            {
                return true;
            }
        }

        return false;
    }

    bool isDoor(char tile) const
    {
        return tile >= 'A' && tile <= 'Z';
    }

    char getKeyForDoor(char door) const
    {
        return std::tolower(door);
    }
};

int lengthOfShortestPathWithAllKeys(const std::vector<std::string>& vaultLines)
{
    VaultTraverser traverser{vaultLines};

    traverser.traverseAllKeys();

    return traverser.getShortestPathLength();
}

}
