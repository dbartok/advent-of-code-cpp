#include "Day7-RecursiveCircus.h"

#include "WeightedTree.h"

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_set>

namespace AdventOfCode
{

std::string nameOfBottomProgram(const std::vector<NodeDescriptor>& nodeDescriptors)
{
    std::vector<std::string> allNodeNames;
    std::transform(nodeDescriptors.begin(), nodeDescriptors.end(), std::back_inserter(allNodeNames),
        [](const NodeDescriptor& nd)
    {
        return nd.name;
    });

    std::vector<std::string> allChildrenNames;
    std::for_each(nodeDescriptors.begin(), nodeDescriptors.end(), [&allChildrenNames](const NodeDescriptor& nd)
    {
        allChildrenNames.insert(allChildrenNames.end(), nd.childrenNames.begin(), nd.childrenNames.end());
    });

    // Calculate set difference between allNodeNames and allChildrenNames
    // Because the elements are not already sorted, using unordered_set + find() is more effective than sorting and using std::set_difference
    std::unordered_set<std::string> allChildrenNamesSet(allChildrenNames.begin(), allChildrenNames.end());

    std::vector<std::string> nodesThatAreNotChildren;
    std::copy_if(allNodeNames.begin(), allNodeNames.end(), std::back_inserter(nodesThatAreNotChildren), 
        [&allChildrenNamesSet](const std::string& name)
    {
        return allChildrenNamesSet.find(name) == allChildrenNamesSet.end();
    });

    if (nodesThatAreNotChildren.size() != 1)
    {
        throw std::runtime_error("There are either 0 or more than 1 bottom programs.");
    }

    return nodesThatAreNotChildren.front();
}

int correctWeightOfWrongWeight(const std::vector<NodeDescriptor>& nodeDescriptors)
{
    WeightedTree weightedTree = WeightedTree::fromNodeDescriptors(nodeDescriptors);
    return weightedTree.getCorrectWeightForSingleWrongWeight();
}

}


int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<AoC::NodeDescriptor> nodeDescriptors;

    std::string lineBuffer;
    while (std::getline(fileIn, lineBuffer))
    {
        AoC::NodeDescriptor nodeDescriptor = AoC::NodeDescriptor::fromString(lineBuffer);
        nodeDescriptors.push_back(std::move(nodeDescriptor));
    }

    std::cout << "First part: " << AoC::nameOfBottomProgram(nodeDescriptors) << std::endl;
    std::cout << "Second part: " << AoC::correctWeightOfWrongWeight(nodeDescriptors) << std::endl;
}
