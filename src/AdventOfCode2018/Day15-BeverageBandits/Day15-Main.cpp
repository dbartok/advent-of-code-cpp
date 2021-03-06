#include "Day15-BeverageBandits.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");

    std::string lineBuffer;
    std::vector<std::string> lines;
    while (std::getline(fileIn, lineBuffer))
    {
        if (!lineBuffer.empty())
        {
            lines.push_back(std::move(lineBuffer));
        }
    }

    std::cout << "First part: " << AoC::outcomeOfCombat(lines) << std::endl;
    std::cout << "Second part: " << AoC::outcomeOfCombatIfElvesBarelyWin(lines) << std::endl;
}
