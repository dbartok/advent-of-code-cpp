#include "Day21-FractalArt.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<std::string> inputEnhancementRuleLines;

    std::string lineBuffer;
    while (std::getline(fileIn, lineBuffer))
    {
        inputEnhancementRuleLines.push_back(std::move(lineBuffer));
    }

    std::cout << "First part: " << AoC::numPixelsOnSmallIterations(inputEnhancementRuleLines) << std::endl;
    std::cout << "Second part: " << AoC::numPixelsOnLargeIterations(inputEnhancementRuleLines) << std::endl;
}
