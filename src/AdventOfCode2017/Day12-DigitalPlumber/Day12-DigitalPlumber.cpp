#include "Day12-DigitalPlumber.h"

#include <fstream>
#include <iostream>
#include <string>

namespace AdventOfCode
{

unsigned numNodesInGroupNodeZero(const std::vector<std::string>& neighborsLines)
{
    VillageGraph villageGraph = VillageGraph::fromNeighborsLines(neighborsLines);
    return villageGraph.numNodesInGroupContainingNode(0);
}

unsigned numTotalGroups(const std::vector<std::string>& neighborsLines)
{
    VillageGraph villageGraph = VillageGraph::fromNeighborsLines(neighborsLines);
    return villageGraph.numTotalGroups();
}

}


int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<std::string> inputNeighborsLines;

    std::string lineBuffer;
    while (std::getline(fileIn, lineBuffer))
    {
        inputNeighborsLines.push_back(lineBuffer);
    }

    std::cout << "First part: " << AoC::numNodesInGroupNodeZero(inputNeighborsLines) << std::endl;
    std::cout << "Second part: " << AoC::numTotalGroups(inputNeighborsLines) << std::endl;
}
