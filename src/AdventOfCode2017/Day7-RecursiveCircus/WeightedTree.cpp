#include "WeightedTree.h"

#include "../../Common/Utils.h"

#include <unordered_map>
#include <unordered_set>
#include <numeric>

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
    for (const auto& childSharedPtr : nodeSharedPtr->childrenSharedPtrs)
    {
        recalculateSubtreeWeights(childSharedPtr);
    }

    nodeSharedPtr->totalSubtreeWeight = std::accumulate(nodeSharedPtr->childrenSharedPtrs.cbegin(), nodeSharedPtr->childrenSharedPtrs.cend(), nodeSharedPtr->weight,
        [](int sum, const TreeNodeSharedPtr& childSharedPtr)
    {
        return sum + childSharedPtr->totalSubtreeWeight;
    });
}

boost::optional<int> WeightedTree::traverseForCorrectWeight(const TreeNodeSharedPtr& nodeSharedPtr, int weightExtra) const
{
    const std::vector<TreeNodeSharedPtr>& childrenSharedPtrs = nodeSharedPtr->childrenSharedPtrs;
    bool balancedSoFar = (weightExtra == 0);

    // Exactly one child, skip forward
    if (childrenSharedPtrs.size() == 1)
    {
        return traverseForCorrectWeight(childrenSharedPtrs.front(), weightExtra);
    }

    bool areAllChildrenEqualSubtreeWeight = Utils::allElementsEqual(childrenSharedPtrs.cbegin(), childrenSharedPtrs.cend(), 
        [](const TreeNodeSharedPtr& lhs, const TreeNodeSharedPtr& rhs)
    {
        return lhs->totalSubtreeWeight == rhs->totalSubtreeWeight;
    });


    // Children are balanced, meaning there are no further imbalances down the tree
    if (areAllChildrenEqualSubtreeWeight)
    {
        // There weren't any imbalances to begin with, so there is no weight to balance in this subtree
        if (weightExtra == 0)
        {
            return boost::none;
        }
        // This is the node that has to be balanced, return the correct weight for it
        else
        {
            return nodeSharedPtr->weight - weightExtra;
        }
    }

    // Two imbalanced children
    if (childrenSharedPtrs.size() == 2)
    {
        std::vector<TreeNodeSharedPtr>::const_iterator minIter, maxIter;
        std::tie(minIter, maxIter) = std::minmax_element(childrenSharedPtrs.cbegin(), childrenSharedPtrs.cend(), &WeightedTree::lessCompareNodeSharedPtrWeight);
        int weightExtraMin = (*minIter)->totalSubtreeWeight - (*maxIter)->totalSubtreeWeight;
        int weightExtraMax = (*maxIter)->totalSubtreeWeight - (*minIter)->totalSubtreeWeight;

        // Balanced so far, traverse both ways
        if (weightExtra == 0)
        {
            boost::optional<int> traverseMinResult = traverseForCorrectWeight(*minIter, weightExtraMin);
            boost::optional<int> traverseMaxResult = traverseForCorrectWeight(*maxIter, weightExtraMax);

            if (traverseMaxResult.is_initialized() && traverseMinResult.is_initialized())
            {
                throw std::runtime_error("A valid corrected value was found for more than one weight.");
            }

            return traverseMinResult.is_initialized() ? traverseMinResult : traverseMaxResult;
        }
        // There is already an imbalance, traverse only in the direction where the same imbalance could still be corrected
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
                throw std::runtime_error("Imbalanced node with two children found in an already differently imbalanced branch.");
            }
        }
    }

    // Three or more children with an imbalance somewhere
    if (childrenSharedPtrs.size() >= 3)
    {
        // Determine child with the unique subtree weight, because that's the direction where the imbalance is
        // We also need the value of the non-unique subtree weight so that we know the weight difference
        std::unordered_map<int, TreeNodeSharedPtr> nodeSharedPtrToSubtreeWeightMap; // Nodes with potentially unique subtree weight
        std::unordered_set<int> uniqueWeightSet;
        int nonUniqueSubtreeWeight = -1;

        for (const auto& childSharedPtr : childrenSharedPtrs)
        {
            bool insertionTookPlace;
            std::tie(std::ignore, insertionTookPlace) = uniqueWeightSet.insert(childSharedPtr->totalSubtreeWeight);

            // This child doesn't have a unique subtree weight
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

        // Only the child with the unique subtere weight remains in the map at this point
        if (nodeSharedPtrToSubtreeWeightMap.size() != 1)
        {
            throw std::runtime_error("Node with three or more children but no single unique subtree weight found.");
        }

        assert(nonUniqueSubtreeWeight != -1);

        TreeNodeSharedPtr uniqueWeightNodeSharedPtr = nodeSharedPtrToSubtreeWeightMap.cbegin()->second;
        int imbalancedChildWeightExtra = uniqueWeightNodeSharedPtr->totalSubtreeWeight - nonUniqueSubtreeWeight;

        if (weightExtra != 0 && imbalancedChildWeightExtra != weightExtra)
        {
            throw std::runtime_error("Imbalanced node with three or more children found in an already differently imbalanced branch.");
        }

        // Traverse in the only direction where the imbalance can be corrected
        return traverseForCorrectWeight(uniqueWeightNodeSharedPtr, imbalancedChildWeightExtra);
    }
}

int WeightedTree::getRightWeightForSingleWrongWeight() const
{
    if (m_root == nullptr)
    {
        throw std::runtime_error("Tree is empty.");
    }

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
            nodeSharedPtr->childrenSharedPtrs.push_back(allNodes.at(childName));

            allNodesWithoutParents.erase(childName);
        }
    }

    if (allNodesWithoutParents.size() != 1)
    {
        throw std::runtime_error("There are either zero or more than one tree roots.");
    }

    TreeNodeSharedPtr root = allNodesWithoutParents.cbegin()->second;
    return WeightedTree{std::move(root)};
}

}
