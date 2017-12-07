#include "Day7-RecursiveCircus.h"

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <unordered_set>

namespace AdventOfCode
{

std::string nameOfBottomProgram(const std::vector<NodeDescriptor>& nodeDescriptors)
{
    std::vector<std::string> allParentNames;
    std::transform(nodeDescriptors.begin(), nodeDescriptors.end(), std::back_inserter(allParentNames),
        [](const NodeDescriptor& nd)
    {
        return nd.parentName;
    });

    std::vector<std::string> allChildrenNames;
    std::for_each(nodeDescriptors.begin(), nodeDescriptors.end(), [&allChildrenNames](const NodeDescriptor& nd)
    {
        allChildrenNames.insert(allChildrenNames.end(), nd.childrenNames.begin(), nd.childrenNames.end());
    });

    // Calculate set difference between allParentNames and allChildrenNames
    // Because the elements are not already sorted, using unordered_set + find() is more effective than sorting and using std::set_difference
    std::unordered_set<std::string> allChildrenNamesSet(allChildrenNames.begin(), allChildrenNames.end());

    std::vector<std::string> parentsThatAreNotChildren;
    std::copy_if(allParentNames.begin(), allParentNames.end(), std::back_inserter(parentsThatAreNotChildren), 
        [&allChildrenNamesSet](const std::string& name)
    {
        return allChildrenNamesSet.find(name) == allChildrenNamesSet.end();
    });

    if (parentsThatAreNotChildren.size() != 1)
    {
        throw std::runtime_error("There are either 0 or more than one bottom programs.");
    }

    return parentsThatAreNotChildren.front();
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
}
