#include "Day09-StreamProcessing.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::string groupedStringInput;

    fileIn >> groupedStringInput;

    std::cout << "First part: " << AoC::totalScoreForAllGroups(groupedStringInput) << std::endl;
    std::cout << "Second part: " << AoC::totalGarbageCount(groupedStringInput) << std::endl;
}
