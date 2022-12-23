//TODO: Rename to current day and name of the puzzle
#include "Day22-MonkeyMap.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

unsigned CUBE_SIZE = 50;

}

int main()
{
    namespace CurrentDay = AdventOfCode::Year2022::Day22;

    std::fstream fileIn("input.txt");

    std::string lineBuffer;
    std::vector<std::string> lines;
    while (std::getline(fileIn, lineBuffer))
    {
        lines.push_back(std::move(lineBuffer));
    }

    std::cout << "First part: " << CurrentDay::finalPassword(lines) << std::endl;
    std::cout << "Second part: " << CurrentDay::finalPasswordWithCubeMap(lines, CUBE_SIZE) << std::endl;
}
