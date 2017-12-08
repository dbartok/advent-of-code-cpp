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
private:
    struct TreeNode;
    using TreeNodeSharedPtr = std::shared_ptr<TreeNode>;

    struct TreeNode
    {
        std::string name;
        int weight;
        std::vector<TreeNodeSharedPtr> childrenSharedPtrs;

        int totalSubtreeWeight = 0;

        TreeNode(std::string name, int weight);
    };

    static bool lessCompareNodeSharedPtrWeight(const TreeNodeSharedPtr& lhs, const TreeNodeSharedPtr& rhs)
    {
        return lhs->weight < rhs->weight;
    };

    TreeNodeSharedPtr m_root;

    void recalculateSubtreeWeights(const TreeNodeSharedPtr& nodeSharedPtr);

    // Recursively traverse a non-empty tree to find a single weight that causes an imbalance
    // Returns the right weight value for the weight if found or boost::none if no such weight was found
    boost::optional<int> traverseForCorrectWeight(const TreeNodeSharedPtr& nodeSharedPtr, int difference) const;
public:
    WeightedTree(TreeNodeSharedPtr m_root);

    // Returns the right weight value for the single weight that causes an imbalance
    int getRightWeightForSingleWrongWeight() const;
    
    static WeightedTree fromNodeDescriptors(const std::vector<NodeDescriptor>& nodeDescriptors);
};

}
