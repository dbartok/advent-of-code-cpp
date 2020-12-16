#include "Day15-RambunctiousRecitation.h"

#include <AdventOfCodeCommon/Utils.h>

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
    const std::string intcodeProgramCommaSeparated
    {
        (std::istreambuf_iterator<char>(fileIn)),
        (std::istreambuf_iterator<char>())
    };

    std::vector<int> startingNumbers;
    AoC::Utils::splitStringIntoTypedVector(intcodeProgramCommaSeparated, startingNumbers, ',');

    std::cout << "First part: " << AoC::nthNumberSpokenSmallN(startingNumbers) << std::endl;
    std::cout << "Second part: " << AoC::nthNumberSpokenLargeN(startingNumbers) << std::endl;
}
