#include "Day07-RecursiveCircus.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

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
