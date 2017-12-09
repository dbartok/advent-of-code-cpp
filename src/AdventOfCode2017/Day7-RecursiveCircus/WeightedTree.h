#pragma once

#include "NodeDescriptor.h"

#include <boost/optional.hpp>

#include <string>
#include <vector>
#include <memory>

namespace AdventOfCode
{

class WeightedTree
{
public:
    struct TreeNode;
    using TreeNodeSharedPtr = std::shared_ptr<TreeNode>;
    using TreeNodeWeakPtr = std::weak_ptr<TreeNode>;

    struct TreeNode
    {
        std::string name;
        int weight;

        std::vector<TreeNodeSharedPtr> childrenSharedPtrs;
        boost::optional<TreeNodeWeakPtr> parentWeakPtr;

        int totalSubtreeWeight;

        TreeNode(std::string name, int weight);

        // Returns the sibling count for the node, including itself in the count
        int totalSiblingCount() const;
    };

    WeightedTree(TreeNodeSharedPtr m_root);

    // Returns the correct weight value for the single weight that causes an imbalance
    int getCorrectWeightForSingleWrongWeight() const;

    static WeightedTree fromNodeDescriptors(const std::vector<NodeDescriptor>& nodeDescriptors);

private:
    TreeNodeSharedPtr m_root;

    void recalculateSubtreeWeights(const TreeNodeSharedPtr& nodeSharedPtr);

    // Recursively traverse a non-empty tree to find a single weight that causes an imbalance
    // Returns the correct weight value for the single wrong weight
    int traverseForCorrectWeight(const TreeNodeSharedPtr& nodeSharedPtr, int weightSurplus) const;
};

}
