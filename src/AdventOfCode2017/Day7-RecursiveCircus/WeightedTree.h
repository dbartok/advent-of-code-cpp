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
        std::vector<TreeNodeSharedPtr> children;

        int totalSubtreeWeight = 0;

        TreeNode(std::string name, int weight);
    };

    static bool lessCompareNodeSharedPtrWeight(const TreeNodeSharedPtr& lhs, const TreeNodeSharedPtr& rhs)
    {
        return lhs->weight < rhs->weight;
    };

    static bool equalsNodeSharedPtrTotalSubtreeWeight(const TreeNodeSharedPtr& lhs, const TreeNodeSharedPtr& rhs)
    {
        return lhs->totalSubtreeWeight == rhs->totalSubtreeWeight;
    };

    TreeNodeSharedPtr m_root;

    void recalculateSubtreeWeights(const TreeNodeSharedPtr& nodeSharedPtr);
    boost::optional<int> traverseForCorrectWeight(const TreeNodeSharedPtr& nodeSharedPtr, int difference);
public:
    WeightedTree(TreeNodeSharedPtr m_root);
    int getRightWeightForSingleWrongWeight();
    
    static WeightedTree fromNodeDescriptors(const std::vector<NodeDescriptor>& nodeDescriptors);
};

}
