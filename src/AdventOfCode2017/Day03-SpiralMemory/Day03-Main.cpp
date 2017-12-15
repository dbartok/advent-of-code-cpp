#include "Day03-SpiralMemory.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    unsigned input;

    fileIn >> input;

    std::cout << "First part: " << AoC::stepsToCarryBack(input) << std::endl;
    std::cout << "Second part: " << AoC::stressTestFirstValueGreaterThan(input) << std::endl;
}
