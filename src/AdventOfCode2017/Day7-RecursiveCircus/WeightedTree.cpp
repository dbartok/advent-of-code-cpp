#include "WeightedTree.h"

#include <unordered_map>
#include <unordered_set>

namespace AdventOfCode
{

WeightedTree::TreeNode::TreeNode(std::string name, int weight)
    : name(std::move(name))
    , weight(weight)
{

}

WeightedTree::WeightedTree(TreeNodeSharedPtr root)
    : m_root(std::move(root))
{
    recalculateSubtreeWeights(m_root);
}

void WeightedTree::recalculateSubtreeWeights(const TreeNodeSharedPtr& nodeSharedPtr)
{
    nodeSharedPtr->totalSubtreeWeight = nodeSharedPtr->weight;

    for (const auto& childSharedPtr : nodeSharedPtr->children)
    {
        recalculateSubtreeWeights(childSharedPtr);
    }

    for (const auto& childSharedPtr : nodeSharedPtr->children)
    {
        nodeSharedPtr->totalSubtreeWeight += childSharedPtr->totalSubtreeWeight;
    }
}

boost::optional<int> WeightedTree::traverseForCorrectWeight(const TreeNodeSharedPtr& nodeSharedPtr, int weightExtra)
{
    const std::vector<TreeNodeSharedPtr>& children = nodeSharedPtr->children;

    // TODO: Can be extracted to Utils
    if (children.size() == 0 || std::equal(children.cbegin() + 1, children.cend(), children.cbegin(), &WeightedTree::equalsNodeSharedPtrTotalSubtreeWeight))
    {
        return nodeSharedPtr->weight - weightExtra;
    }

    if (children.size() == 1)
    {
        return traverseForCorrectWeight(nodeSharedPtr->children.front(), weightExtra);
    }

    if (children.size() == 2)
    {
        std::vector<TreeNodeSharedPtr>::const_iterator minIter, maxIter;
        std::tie(minIter, maxIter) = std::minmax_element(children.begin(), children.end(), &WeightedTree::lessCompareNodeSharedPtrWeight);
        int weightExtraMin = (*minIter)->totalSubtreeWeight - (*maxIter)->totalSubtreeWeight;
        int weightExtraMax = (*maxIter)->totalSubtreeWeight - (*minIter)->totalSubtreeWeight;

        if (weightExtra == 0)
        {
            boost::optional<int> balanceMinResult = traverseForCorrectWeight(*minIter, weightExtraMin);
            boost::optional<int> balanceMaxResult = traverseForCorrectWeight(*maxIter, weightExtraMax);

            if (balanceMaxResult.is_initialized() && balanceMinResult.is_initialized())
            {
                throw std::runtime_error("A valid corrected value was found for more than one weight.");
            }

            return balanceMinResult.is_initialized() ? balanceMinResult : balanceMaxResult;
        }
        else
        {
            if (weightExtraMin == weightExtra)
            {
                return traverseForCorrectWeight(*minIter, weightExtraMin);
            }
            else if (weightExtraMax == weightExtra)
            {
                return traverseForCorrectWeight(*maxIter, weightExtraMax);
            }
            else
            {
                throw std::runtime_error("Unbalanced node with two children found in an already differently unbalanced branch.");
            }
        }
    }

    if (children.size() >= 3)
    {
        std::unordered_map<int, TreeNodeSharedPtr> nodeSharedPtrToSubtreeWeightMap;
        std::unordered_set<int> weightSet;
        int nonUniqueSubtreeWeight = -1;

        for (const auto& childSharedPtr : children)
        {
            bool insertionTookPlace;
            std::tie(std::ignore, insertionTookPlace) = weightSet.insert(childSharedPtr->totalSubtreeWeight);

            if (!insertionTookPlace)
            {
                nonUniqueSubtreeWeight = childSharedPtr->totalSubtreeWeight;
                nodeSharedPtrToSubtreeWeightMap.erase(childSharedPtr->totalSubtreeWeight);
            }
            else
            {
                nodeSharedPtrToSubtreeWeightMap.insert(std::make_pair(childSharedPtr->totalSubtreeWeight, childSharedPtr));
            }
        }

        if (nodeSharedPtrToSubtreeWeightMap.size() != 1)
        {
            throw std::runtime_error("Node with three or more children but no single unique subtree weight found.");
        }

        assert(nonUniqueSubtreeWeight != -1);

        TreeNodeSharedPtr uniqueWeightNodeSharedPtr = nodeSharedPtrToSubtreeWeightMap.begin()->second;
        int newWeightExtra = uniqueWeightNodeSharedPtr->totalSubtreeWeight - nonUniqueSubtreeWeight;

        if (weightExtra != 0 && newWeightExtra != weightExtra)
        {
            throw std::runtime_error("Unbalanced node with three or more children found in an already differently unbalanced branch.");
        }

        return traverseForCorrectWeight(uniqueWeightNodeSharedPtr, newWeightExtra);
    }
}

int WeightedTree::getRightWeightForSingleWrongWeight()
{
    boost::optional<int> traverseResult = traverseForCorrectWeight(m_root, 0);

    if (!traverseResult.is_initialized())
    {
        throw std::runtime_error("Unable to balance tree by modifying one specific weight.");
    }

    return traverseResult.get();
}


WeightedTree WeightedTree::fromNodeDescriptors(const std::vector<NodeDescriptor>& nodeDescriptors)
{
    std::unordered_map<std::string, TreeNodeSharedPtr> allNodes;

    // Create all nodes
    for (const auto& nodeDescriptor : nodeDescriptors)
    {
        TreeNodeSharedPtr newNodeSharedPtr = std::make_shared<TreeNode>(nodeDescriptor.name, nodeDescriptor.weight);

        bool insertionTookPlace;
        std::tie(std::ignore, insertionTookPlace) = allNodes.insert(std::make_pair(newNodeSharedPtr->name, newNodeSharedPtr));

        if (!insertionTookPlace)
        {
            throw std::runtime_error("Node with duplicate name encountered.");
        }
    }

    std::unordered_map<std::string, TreeNodeSharedPtr> allNodesWithoutParents = allNodes;


    // Set children
    for (const auto& nodeDescriptor : nodeDescriptors)
    {
        const std::string& name = nodeDescriptor.name;
        const TreeNodeSharedPtr& nodeSharedPtr = allNodes.at(name);

        for (const auto& childName : nodeDescriptor.childrenNames)
        {
            nodeSharedPtr->children.push_back(allNodes.at(childName));

            allNodesWithoutParents.erase(childName);
        }
    }

    if (allNodesWithoutParents.size() != 1)
    {
        throw std::runtime_error("There are either zero or more than one tree roots.");
    }

    TreeNodeSharedPtr root = allNodesWithoutParents.begin()->second;
    return WeightedTree{std::move(root)};
}

}
