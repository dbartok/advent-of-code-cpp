#include "Day07-AmplificationCircuit.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
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

    std::vector<int> intcodeProgram;
    AoC::Utils::splitStringIntoTypedVector(intcodeProgramCommaSeparated, intcodeProgram, ',');

    std::cout << "First part: " << AoC::highestPossibleSignalSent(intcodeProgram) << std::endl;
}
