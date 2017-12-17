#include "Day17-Spinlock.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    unsigned inputStepsForward;

    fileIn >> inputStepsForward;

    std::cout << "First part: " << AoC::valueAfterLastWritten(inputStepsForward) << std::endl;
    std::cout << "Second part: " << AoC::valueAfterZeroAfterManyInsertions(inputStepsForward) << std::endl;
}
