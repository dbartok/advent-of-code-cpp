#include "Day5-MazeOfTrampolines.h"

#include <fstream>
#include <iostream>

namespace AdventOfCode
{

unsigned jumpInstructionsIncreasing(std::vector<int> instructions)
{
    int position = 0;
    unsigned steps = 0;
    while (position >= 0 && position < instructions.size())
    {
        int newPosition = position + instructions[position];
        ++instructions[position];

        position = newPosition;
        ++steps;
    }

    return steps;
}

}


int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<int> inputInstructions;

    int inputOffset;
    while (fileIn >> inputOffset)
    {
        inputInstructions.push_back(inputOffset);
    }

    std::cout << "First part: " << AoC::jumpInstructionsIncreasing(inputInstructions) << std::endl;
}
