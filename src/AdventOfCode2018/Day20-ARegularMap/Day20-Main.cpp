#include "Day20-ARegularMap.h"

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

    std::string roomLayoutRegex;
    fileIn >> roomLayoutRegex;

    std::cout << "First part: " << AoC::distanceToFarthestRoom(roomLayoutRegex) << std::endl;
    std::cout << "Second part: " << AoC::numRoomsWithLargeDistance(roomLayoutRegex) << std::endl;
}
