#include "DayX-PuzzleName.h" //TODO: Change to the name of the puzzle

#include <fstream>
#include <iostream>

namespace AdventOfCode
{

//TODO: Implement actual functions
int solution1(int input)
{
    return input + 1;
}

int solution2(int input)
{
    return input + 2;
}

}


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
