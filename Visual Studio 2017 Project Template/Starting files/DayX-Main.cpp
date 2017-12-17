#include "DayX-PuzzleName.h" //TODO: Change to the name of the puzzle

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
END_LIBRARIES_DISABLE_WARNINGS

//TODO: Implement parsing and driver
int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    int puzzleInput;

    fileIn >> puzzleInput;

    std::cout << "First part: " << AoC::solution1(puzzleInput) << std::endl;
    std::cout << "Second part: " << AoC::solution2(puzzleInput) << std::endl;
}
